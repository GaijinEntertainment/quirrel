---
title: Values and types
group: Language
order: 25
summary: The fourteen types, and which of them are references.
---

Quirrel is dynamically typed: a variable has no type of its own, only the
value in it does. [type](sym:type) reports that value's type as one of
fourteen strings.

## The fourteen types

[null](sym:types.Null), [bool](sym:types.Bool), [integer](sym:types.Integer),
[float](sym:types.Float), [string](sym:types.String), [table](sym:types.Table),
[array](sym:types.Array), [function](sym:types.Function),
[generator](sym:types.Generator), [userdata](sym:types.UserData),
[thread](sym:types.Thread), [class](sym:types.Class),
[instance](sym:types.Instance), [weakref](sym:types.WeakRef) - each links to
the methods every value of that type has. A native closure reports
`function` too, same as a script one; a class's own `_typeof` metamethod
changes what the `typeof` operator reports, never what `type()` reports
(below).

## Truthiness

Only four values are false in a condition: `null`, `false`, the integer `0`
and the float `0.0`. Everything else is true, which surprises newcomers from
languages where an empty string or an empty container is false - here an
empty string, an empty array and an empty table are all true.

{{example:language/types-truthiness}}

## Integers and floats

An integer is 64 bit. A float, in this build, is single precision -
[getbuildinfo](sym:debug.getbuildinfo)`().floatsize` is 4 - which is a build
choice upstream Quirrel leaves open, not a language guarantee.

[tofloat](sym:types.Integer.tofloat) and
[tointeger](sym:types.Integer.tointeger) convert explicitly; `tointeger`
truncates toward zero rather than rounding. Converting a large integer to
float is where precision is lost: single precision keeps only 24 bits of
mantissa, so an integer past 2^24 can round to its neighbor, and a mixed
`int == float` comparison converts the integer through that same lossy cast
before comparing - so a big integer can compare equal to a float that is not
actually its value.

{{example:language/types-precision}}

## Equality and identity

`==` on two numbers compares by value regardless of integer versus float, and
on two strings compares by content. On two tables, arrays or instances, it
compares by identity: two separately built tables with identical content are
not `==`, only a table compared with itself (or an alias of it) is. `null` is
its own type and equals only `null` - it is not equal to `false` or `0`,
unlike a loosely-typed language. `<=>` returns -1, 0 or 1 for ordering, and
works on strings as well as numbers.

{{example:language/types-equality}}

## typeof, type, and classof

They are not even the same kind of thing. [type](sym:type) is an ordinary
function, so it can be stored in a binding and passed to
[map](sym:types.Array.map) like any other value. `typeof` is an operator, so
`let describe = typeof` does not compile - it needs an operand. Writing
`typeof(v)` works, but those parentheses are around `v`, not a call.

{{example:language/types-type-vs-typeof}}

Three different questions about a value's type:

- [type](sym:type)`(v)` is the plain engine category from the list above.
  It never consults a metamethod.
- `typeof v` is the same, unless `v`'s class defines `_typeof`, in which
  case that method's result wins. Use `type()` when the raw category
  matters and `typeof` is not what you want.
- [classof](sym:types.classof)`(v)` returns the class object itself: for a
  scalar or container it is the built-in delegate (`Integer`, `String`, ...,
  imported from `"types"`), and for a script class instance it is that exact
  class.

An instance of a script class is the one place `instanceof` misleads: it only
walks the script class hierarchy, so `instance instanceof` the built-in
[Instance](sym:types.Instance) class is always false. Test `type(v) ==
"instance"` instead when the check needs to be "is this any class instance at
all".

{{example:language/types-typeof-classof}}
