"""Extracts the C API from the public headers.

    python gen/dump_capi.py > gen/capi_dump.jsonl

The script pages take their signatures from a running VM, and the C API pages take
theirs from here, for the same reason: a signature nobody typed cannot disagree with
the implementation. Prose stays in content/_capi.json, keyed by function name.

The headers group their own declarations with `/*section*/` markers, and that
grouping is what the pages use, so a function added to a new section shows up in the
right place without anyone editing a page.
"""

import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
INCLUDE = os.path.join(HERE, "..", "..", "include")

# Header -> the group a declaration falls into before any /*section*/ marker. The
# std headers have no markers at all, so this is the only grouping they get.
HEADERS = [
    ("squirrel.h", "vm"),
    ("sqstdaux.h", "stdaux"),
    ("sqstdblob.h", "stdblob"),
    ("sqstddatetime.h", "stddatetime"),
    ("sqstddebug.h", "stddebug"),
    ("sqstdio.h", "stdio"),
    ("sqstdmath.h", "stdmath"),
    ("sqstdstring.h", "stdstring"),
    ("sqstdsystem.h", "stdsystem"),
    ("sqasync.h", "async"),
    ("sqext.h", "ext"),
]

_SECTION = re.compile(r"^/\*([a-z][^*]*)\*/\s*$")

# The reference-count helpers sit at the very end of squirrel.h, past the last
# section marker, so they would otherwise inherit whatever section came last.
REGROUP = {
    "sq_addref": "raw object handling",
    "sq_release": "raw object handling",
}
_NAMESPACE = re.compile(r"^namespace\s+(\w+)")
# The name is the last identifier before the argument list, so a return type that
# carries a namespace or a `*` does not have to be understood to be skipped over.
_DECL = re.compile(r"^SQUIRREL_API\s+(?P<head>.+?)(?P<name>[A-Za-z_]\w*)\s*\((?P<args>.*)\)\s*;$",
                   re.DOTALL)


def split_params(text):
    """Splits an argument list on the commas that separate parameters.

    Depth tracking is needed because a function-pointer parameter carries its own
    parentheses and its own commas."""
    out, depth, cur = [], 0, ""
    for ch in text:
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return [re.sub(r"\s+", " ", p) for p in out if p and p != "void"]


def read_header(name, default_group):
    path = os.path.join(INCLUDE, name)
    with open(path, encoding="utf-8", errors="replace") as f:
        lines = f.read().splitlines()

    rows, group, i = [], default_group, 0
    # Namespace depth only. `extern "C"` and function bodies also carry braces, so
    # the counter tracks the namespace stack rather than every block: a namespace is
    # opened by its own `namespace X {` line and closed when the brace count taken
    # from that point returns to zero.
    namespaces = []     # (name, brace balance owed before it closes)
    pending_namespace = None

    def consume(text):
        """Feeds one source line to the namespace tracker."""
        nonlocal pending_namespace
        for ch in text:
            if ch == "{":
                if pending_namespace is not None:
                    namespaces.append([pending_namespace, 0])
                    pending_namespace = None
                    continue
                if namespaces:
                    namespaces[-1][1] += 1
            elif ch == "}" and namespaces:
                if namespaces[-1][1] == 0:
                    namespaces.pop()
                else:
                    namespaces[-1][1] -= 1

    comment = []
    blank_before = True
    while i < len(lines):
        raw = lines[i].strip()
        line = lines[i].split("//")[0].rstrip()
        # A run of // lines directly above a declaration is that declaration's
        # documentation. Some headers, sqasync.h in particular, carry the only
        # description a function has, so taking it here saves copying it by hand.
        # The run has to start on its own, after a blank line: a comment written
        # straight under a declaration is a note about that one, not about the next.
        if raw.startswith("//") and not line.strip():
            if comment or blank_before:
                comment.append(raw.lstrip("/").strip())
            i += 1
            continue
        blank_before = not raw
        m = _SECTION.match(lines[i].strip())
        if m:
            # A marker may explain itself after a colon; the label is the part before.
            group = m.group(1).split(":")[0].strip()
            i += 1
            continue
        m = _NAMESPACE.match(lines[i].strip())
        if m:
            pending_namespace = m.group(1)
        note = " ".join(comment).strip()
        comment = []
        exported = line.startswith("SQUIRREL_API")
        # Part of the API is `static inline` wrappers over the exported entry points
        # (sq_addref over sq_addref_refcounted, and so on). A caller uses them the
        # same way, so leaving them out would make this list look complete when it
        # was not.
        inline = line.startswith("static inline")
        if not exported and not inline:
            consume(line)
            i += 1
            continue
        namespace = "::".join(n for n, _b in namespaces)
        # A declaration may wrap; join until it closes. An exported one ends at the
        # semicolon, an inline one at the brace that opens its body.
        stop = ";" if exported else "{"
        decl = line
        while stop not in decl and i + 1 < len(lines):
            i += 1
            decl += " " + lines[i].split("//")[0].strip()
        i += 1
        decl = re.sub(r"\s+", " ", decl).strip()
        if inline:
            # Skip the body, so its braces never reach the namespace tracker.
            body = decl[decl.index("{"):]
            balance = body.count("{") - body.count("}")
            while balance > 0 and i < len(lines):
                text = lines[i].split("//")[0]
                balance += text.count("{") - text.count("}")
                i += 1
            decl = "SQUIRREL_API " + decl[len("static inline"):decl.index("{")].strip() + ";"
        else:
            consume(decl)
        if "= delete" in decl:
            # An overload declared only to be rejected at compile time. It is not
            # part of the API, and listing it would read as if it were.
            continue
        m = _DECL.match(decl)
        if not m:
            print(f"warning: {name}: cannot parse {decl!r}", file=sys.stderr)
            continue
        rows.append({
            "header": name,
            "group": REGROUP.get(m.group("name"), group),
            "name": m.group("name"),
            "qualified": f"{namespace}::{m.group('name')}" if namespace else m.group("name"),
            "ret": m.group("head").strip(),
            "params": split_params(m.group("args")),
            "note": note,
        })
    return rows


def main():
    seen = set()
    for name, default_group in HEADERS:
        for row in read_header(name, default_group):
            # Keyed on the whole signature, so a genuine overload survives while a
            # declaration repeated in two headers does not.
            key = (row["qualified"], tuple(row["params"]))
            if key in seen:
                continue
            seen.add(key)
            print(json.dumps(row, separators=(",", ":")))


if __name__ == "__main__":
    main()
