---
title: Operators and expressions
group: Language
order: 35
summary: Precedence, what counts as false, and the null-safe operators.
---

Most operators do what the same symbol does in C. The differences are what
counts as false, what `??` and `?.` check instead, and a handful of
precedence traps.

## All operators

Every operator the compiler accepts, with the metamethod that overloads it where
there is one. Each links to the section that explains it.

{{operators}}

There is no bitwise compound assignment: `&=`, `|=`, `^=`, `<<=` and `>>=` are all
compile errors. Write `flags = flags & mask` instead.

## Arithmetic

`+ - * /` and `%` work on `int` and `float`. Dividing two `int` truncates
toward zero rather than rounding, and mixing an `int` with a `float`
promotes the result to `float`. `%` keeps the sign of the left operand, like
C, not the mathematical modulo.

`+` also concatenates: if either side is a string, it stringifies the other
side instead of adding, so `1 + "2"` is `"12"` rather than an error. Prefer
`$"...{...}"` interpolation for building strings.

{{example:language/operators-arithmetic}}

## Comparison and three-way compare

`== != < <= > >=` compare two values and return a real `bool`, not `0`/`1`.
`<=>` is the three-way compare: it returns an `int` that is negative, zero,
or positive depending on whether the left side is less than, equal to, or
greater than the right, which is exactly the shape [sort](sym:types.Array.sort)
wants from a comparator.

{{example:language/operators-compare}}

## Logical operators

`&&`, `||` and `!` treat `null`, `false`, the integer `0` and the float `0.0`
as false. Everything else is true, including `""`, `[]` and `{}` - Quirrel
does not treat an empty string, array or table as falsy.

`&&` and `||` short-circuit and evaluate to one of their operands, not
necessarily a `bool`: `&&` evaluates its left side, and if that is false,
stops there and yields it; otherwise it evaluates and yields the right side.
`||` is the mirror image. Only `!` always produces a real `bool`.

{{example:language/operators-logical}}

## Null-coalescing and null-safe access

`??` looks like `||` but checks something different: it tests strictly for
`null`; the falsy-but-legitimate values above (`0`, `0.0`, `false`) survive
it. A plain `||` used as a default - a common habit from other languages -
silently replaces a real `0` or `false` with the fallback, which `??` does
not.

`?.` and `?[` are the null-safe forms of `.` and `[`: if the value on the
left is `null`, the whole expression is `null` instead of throwing. Once one
of them fires on a `null`, the compiler treats the rest of the chain -
further `.`, `[`, or a call - as null-safe too, so `a?.b.c[0]()` needs only
one `?.`, not one at every step.

{{example:language/operators-nullsafe}}

## in, instanceof, typeof

`key in container` and `key not in container` test whether a slot exists.
On a table that means the key is present; on an array it means the key is a
valid index, which is a common trap - `20 in [10, 20, 30]` is `false`
because `20` is not a valid index, even though it is one of the values. Use
[contains](sym:types.Array.contains) to search by value instead.

`instanceof` tests whether an instance was made from a class or one of its
subclasses. `typeof` returns the type name as a string (`"table"`,
`"instance"`, `"array"`, ...), through the `_typeof`
[metamethod](page:language/metamethods#type-name-and-text) when the class defines
one. [type](sym:type) answers the same question as a function: it can be passed as
a callback, and it reports the plain type whatever `_typeof` says.

{{example:language/operators-membership}}

## The .$ type-method operator

A table's own slots and its built-in methods share one namespace, so a slot named
`len` or `rawdelete` hides the method of that name. `o.$name(...)` reaches the
built-in type method directly and never sees the slot. The null-safe form,
`o?.$name(...)`, works the same way.

{{example:language/operators-typemethod}}

Plain `o.name(...)` is fine on data whose keys you control. On anything parsed from
a file, received over a network, or handed in by a caller, `.$` is the form that
cannot be broken by the data. This is why the compiler's own advice for the
forbidden `delete` operator names it: `Use 'o.$rawdelete("key")' instead`. See
[types.Table.rawdelete](sym:types.Table.rawdelete).

## clone

`clone value` makes a shallow copy of a table, array, or instance: the
top-level slots are copied, but a container nested inside is shared with the
original.

`clone` is a keyword, not an identifier, so there is no `.clone()` method to
call by default:

```nut
let copy = original.clone()  // parse error: expected 'IDENTIFIER'
```

Write `clone original` instead.

{{example:language/operators-clone}}

## The newslot operator, <-

`<-` adds a new field to a table or instance. If the slot already exists,
`<-` behaves exactly like `=`. The reverse is not true: plain `=` never
creates a slot, so assigning to a field that does not exist yet throws
rather than adding it - `<-` is the only way to add one.

`<-` needs a table or instance slot on its left, not a bare variable: `local
a; a <- 1` is a compile error ("can't 'create' a local slot"), because a
local binding is not a container that can grow new slots.

{{example:language/operators-newslot}}

## Compound assignment and increment/decrement

`+= -= *= /= %=` read the current value, combine it, and write it back, so
(like plain `=`) they need the slot to already exist. `++` and `--` work as
in C: used as a statement the distinction rarely matters, but as an
expression the prefix form yields the value after stepping and the postfix
form yields the value before.

{{example:language/operators-compound}}

## Overloading an operator

A class defines a metamethod to give its instances behaviour for an operator. The
name is the operator's metamethod from the table above, and it is an ordinary
method on the class.

{{example:language/operators-overloading}}

Three things to watch in that class:

- **A class cannot name itself inside its own body.** The binding does not exist
  until the declaration finishes, so `MyPoint2(...)` inside `_add` is
  `Unknown variable [MyPoint2]`. `this.getclass()` is the way out, and it also does
  the right thing when a subclass inherits the metamethod. A
  [forward declaration](page:language/bindings) works too, if the name is wanted.
- **One `_cmp` drives the ordering comparisons.** `<`, `<=`, `>` and `>=` all go
  through it. Return a negative number, zero, or a positive one, as `<=>` does.
  `==` and `!=` are not among them: they compare raw identity and no metamethod
  can change that.
- **`_typeof` changes `typeof`, not [type](sym:type).** `typeof muzzle` gives
  `MyPoint2` while `type(muzzle)` still gives `instance`, because `type` ignores
  metamethods on purpose.

An operator with no metamethod on the class throws rather than falling back to a
default, so an instance that defines `_add` but not `_sub` cannot be subtracted.

[Metamethods](page:language/metamethods) has the contract of each one: what it
receives, which operand supplies it, and what the VM does with what it returns.

## The static memoisation operator

`static` has two unrelated meanings. Inside a class body it declares a shared
member, described in
[Classes and instances](page:language/classes#static-members). In an expression it
is the memoisation operator described here.

`static expr` evaluates `expr` the first time control reaches it and caches the
result. Every later pass over the same place in the code reuses the cached value
without evaluating anything. It binds like the other unary operators - `typeof`,
`clone`, unary `-` - so it takes the whole postfix expression after it.

Two consequences follow from "the same place in the code":

- **The cache belongs to the code location, not to the call.** A `static` inside a
  loop body evaluates on the first pass only, and every later iteration gets that
  first value. Two calls to the same function share one cache, because it is one
  location. Two identical `static` expressions written in two places have two
  separate caches.
- **A cached container is frozen.** An `array`, `table`, `instance`, `class` or
  `userdata` result is made immutable exactly as [freeze](sym:freeze) would, so a
  later write throws `trying to modify immutable 'table'`. This is what makes the
  shared value safe to hand out. A number, string or bool has nothing to freeze and
  is cached as is.

{{example:language/operators-static}}

Use it for work that is expensive and answers the same every time: a parsed table,
a lookup built from constants, a colour palette. Do not use it for anything that
depends on state the expression does not name, because nothing re-runs it when that
state changes.

[modules.reset_static_memos](sym:modules.reset_static_memos) empties every cache in
the VM, which is the one way to force re-evaluation. It exists for events like a
script reload or a resolution change, and it is not cheap enough to call often.

## Precedence traps

- `??` binds looser than the comparison operators, so `a ?? b > c`
  parses as `a ?? (b > c)`, not `(a ?? b) > c`.
- Shift operators (`<< >> >>>`) bind tighter than comparisons, so
  `1 << 2 == 4` parses as `(1 << 2) == 4`.
- `&&` binds tighter than `||`, same as in most languages: parenthesize a
  mixed chain if the reading is not obvious at a glance.
- Assignment is a statement, not a nestable expression: `p = q = 5` is a
  compile error ("'=' inside 'expression' is forbidden"). Write two
  statements instead.

{{example:language/operators-precedence}}

## delete

The `delete` operator is forbidden by default:

```nut
delete tbl.someKey
// Usage of 'delete' operator is forbidden. Use 'o.$rawdelete("key")' instead
```

Use `rawdelete` instead, which does the same removal as a regular method
call and returns the value that was removed.

{{example:language/operators-rawdelete}}

See [Errors and exceptions](page:language/errors) for how a caller reacts
to the errors shown above.
