"""Parses the declaration strings the VM reports for native functions.

The grammar mirrors squirrel/compiler/sqtypeparser.cpp, which is what produced
these strings in the first place:

    pure fastcall clamp(x: number, min: number, max: number): number
    instance.search(str: string, [start: int]): table|null
    (table|userdata|instance|class|null).addBlock(arg1): any
    array.slice([arg1: number, arg2: number], ...): any

Keep TYPE_NAMES in sync with rawTypeDecls in sqtypeparser.cpp; the doc pages link
every type name in a signature, and an unknown name must not silently become a
dead link.
"""

import re

ATTRIBUTES = ("pure", "fastcall", "nodiscard")

_FALLBACK_RECEIVER = frozenset({"table", "userdata", "instance", "class", "null"})

# rawTypeDecls in sqtypeparser.cpp: canonical name -> synonyms accepted by the parser.
TYPE_NAMES = {
    "bool": ("boolean",),
    "number": ("num",),
    "int": ("integer",),
    "float": ("double", "real"),
    "string": ("str",),
    "table": ("dict", "map"),
    "array": ("list", "vector"),
    "userdata": ("user", "object"),
    "function": ("func", "closure"),
    "generator": ("gen", "yield"),
    "userpointer": ("ptr", "pointer"),
    "thread": ("coroutine", "fiber"),
    "instance": ("inst", "object"),
    "class": (),
    "weakref": ("reference", "ref"),
    "null": ("nil", "none"),
    "any": (),
}

CANONICAL_TYPE = {}
for _canon, _synonyms in TYPE_NAMES.items():
    CANONICAL_TYPE[_canon] = _canon
    for _syn in _synonyms:
        CANONICAL_TYPE.setdefault(_syn, _canon)


class Param:
    def __init__(self, name, type_names, optional=False, vararg=False):
        self.name = name
        self.types = type_names          # list of canonical type names, [] means untyped
        self.optional = optional
        self.vararg = vararg

    @property
    def type_text(self):
        return "|".join(self.types)

    def __repr__(self):
        return f"Param({self.name!r}, {self.types!r}, optional={self.optional}, vararg={self.vararg})"


class Signature:
    def __init__(self, name, attrs, receiver, params, ret, source):
        self.name = name
        self.attrs = attrs               # subset of ATTRIBUTES, in declaration order
        self.receiver = receiver         # list of type names the method hangs off, or []
        self.params = params             # includes the vararg entry, if any
        self.ret = ret                   # list of canonical type names, [] when not declared
        self.source = source

    @property
    def receiver_unknown(self):
        """True when the receiver is the fixed union sqstddebug.cpp prints for a
        native closure that carries no typecheck at all.

        It means "the VM knows nothing", not "one of these five": it shows up on
        types.Integer.weakref too, and integer is not in the list. The page states
        the real receiver already, so rendering this would only mislead.
        """
        return frozenset(self.receiver) == _FALLBACK_RECEIVER

    @property
    def has_placeholder_names(self):
        """True when the VM synthesized arg1/arg2 names.

        Those come from bindings that were registered with a typemask instead of a
        decl string, so the parameter names on such a page carry no information.
        """
        return any(re.fullmatch(r"arg\d+", p.name) for p in self.params)

    def render(self, link_type=None, link_name=None):
        """Renders the signature as HTML. The callbacks turn a type name or the
        function name into a link; both default to plain text."""
        link_type = link_type or (lambda t: t)

        def types(names):
            return "|".join(link_type(t) for t in names)

        out = []
        for attr in self.attrs:
            out.append(f'<span class="attr">{attr}</span> ')
        if self.receiver and not self.receiver_unknown:
            recv = types(self.receiver)
            if len(self.receiver) > 1:
                recv = f"({recv})"
            out.append(f'<span class="recv">{recv}.</span>')
        out.append(link_name(self.name) if link_name else f'<span class="fn">{self.name}</span>')

        def one(p):
            text = f'<span class="pname">{"..." if p.vararg else p.name}</span>'
            return text + (f": {types(p.types)}" if p.types else "")

        # Contiguous optional parameters were one `[...]` group in the source and
        # must render as one, otherwise `slice([a, b])` reads as two independent
        # optional arguments.
        parts, group = [], []
        for p in self.params:
            if p.optional:
                group.append(one(p))
                continue
            if group:
                parts.append('<span class="opt">[' + ", ".join(group) + "]</span>")
                group = []
            parts.append(one(p))
        if group:
            parts.append('<span class="opt">[' + ", ".join(group) + "]</span>")
        out.append("(" + ", ".join(parts) + ")")

        if self.ret:
            out.append(f": {types(self.ret)}")
        return "".join(out)


class DeclParseError(Exception):
    pass


_IDENT = re.compile(r"[A-Za-z_]\w*")


class _Cursor:
    def __init__(self, text):
        self.text = text
        self.pos = 0

    def skip_spaces(self):
        while self.pos < len(self.text) and self.text[self.pos].isspace():
            self.pos += 1

    def peek(self):
        self.skip_spaces()
        return self.text[self.pos] if self.pos < len(self.text) else ""

    def take(self, ch):
        if self.peek() == ch:
            self.pos += 1
            return True
        return False

    def expect(self, ch):
        if not self.take(ch):
            raise DeclParseError(f"expected {ch!r} at {self.pos} in {self.text!r}")

    def ident(self):
        self.skip_spaces()
        m = _IDENT.match(self.text, self.pos)
        if not m:
            return None
        self.pos = m.end()
        return m.group(0)


def _type_union(cur):
    """Reads `a|b|c` and canonicalizes each name."""
    names = []
    while True:
        name = cur.ident()
        if name is None:
            raise DeclParseError(f"expected a type name at {cur.pos} in {cur.text!r}")
        names.append(CANONICAL_TYPE.get(name, name))
        if not cur.take("|"):
            return names


def _params(cur, params, optional):
    """Reads a comma separated parameter list into `params`.

    A `[...]` group marks its contents optional; the group may be followed by more
    parameters, as in `array.slice([arg1: number, arg2: number], ...)`.
    """
    while True:
        ch = cur.peek()
        if ch in (")", ""):
            return
        if cur.take("["):
            _params(cur, params, True)
            cur.expect("]")
        elif cur.text.startswith("...", cur.pos):
            cur.pos += 3
            types = _type_union(cur) if cur.take(":") else []
            params.append(Param("...", types, optional, vararg=True))
        else:
            name = cur.ident()
            if name is None:
                raise DeclParseError(f"expected a parameter name at {cur.pos} in {cur.text!r}")
            types = _type_union(cur) if cur.take(":") else []
            params.append(Param(name, types, optional))
        if not cur.take(","):
            return


def parse_decl(decl):
    """Parses a decl string into a Signature. Raises DeclParseError on anything
    the grammar does not cover, so a new binding style cannot slip through as a
    silently malformed page."""
    cur = _Cursor(decl)

    attrs = []
    while True:
        save = cur.pos
        word = cur.ident()
        if word in ATTRIBUTES:
            attrs.append(word)
        else:
            cur.pos = save
            break

    receiver = []
    # Receivers appear either bare (`array.sort`) or parenthesized when they are a
    # union (`(table|userdata|instance|class|null).addBlock`).
    save = cur.pos
    if cur.take("("):
        try:
            receiver = _type_union(cur)
            cur.expect(")")
            cur.expect(".")
        except DeclParseError:
            cur.pos = save
            receiver = []
    else:
        head = cur.ident()
        if head is not None and cur.take("."):
            receiver = [CANONICAL_TYPE.get(head, head)]
        else:
            cur.pos = save

    name = cur.ident()
    if name is None:
        raise DeclParseError(f"expected a function name in {decl!r}")

    cur.expect("(")
    params = []
    _params(cur, params, False)
    cur.expect(")")

    ret = _type_union(cur) if cur.take(":") else []

    cur.skip_spaces()
    if cur.pos != len(cur.text):
        raise DeclParseError(f"trailing text at {cur.pos} in {decl!r}")

    return Signature(name, attrs, receiver, params, ret, decl)
