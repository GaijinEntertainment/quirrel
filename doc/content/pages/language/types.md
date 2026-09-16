---
title: Values and types
group: Language
order: 25
summary: The fourteen value types, truthiness, number precision, and equality.
---

Quirrel is dynamically typed: a variable has no type of its own; only the
value in it has a type. [type](sym:type) reports that type as one of
fourteen strings.

## The fourteen types

[null](sym:types.Null), [bool](sym:types.Bool), [integer](sym:types.Integer),
[float](sym:types.Float), [string](sym:types.String), [table](sym:types.Table),
[array](sym:types.Array), [function](sym:types.Function),
[generator](sym:types.Generator), [userdata](sym:types.UserData),
[thread](sym:types.Thread), [class](sym:types.Class),
[instance](sym:types.Instance), [weakref](sym:types.WeakRef). Each name links
to the methods that every value of that type has. A native closure reports
`function`, the same as a script closure. A class's `_typeof` metamethod
changes what the `typeof` operator reports, but never what `type()` reports
(see below).

## Truthiness

Only four values are false in a condition: `null`, `false`, the integer `0`
and the float `0.0`. Everything else is true. Unlike some other languages,
an empty string, an empty array and an empty table are all true.

{{example:language/types-truthiness}}

## Integers and floats

An integer is 64 bit. A float in this build is single precision:
[getbuildinfo](sym:debug.getbuildinfo)`().floatsize` is 4. The float size is
a build option, not a language guarantee.

[tofloat](sym:types.Integer.tofloat) and
[tointeger](sym:types.Integer.tointeger) convert explicitly. `tointeger`
truncates toward zero; it does not round. Conversion of a large integer to
float loses precision. Single precision keeps only 24 bits of mantissa, so
an integer above 2^24 can round to a neighbor value. A mixed `int == float`
comparison converts the integer with that same lossy cast before it
compares, so a big integer can compare equal to a float that does not have
its value.

{{example:language/types-precision}}

## Equality and identity

`==` on two numbers compares by value, whether they are integers or floats.
On two strings it compares by content. On two tables, arrays or instances it
compares by identity: two separately built tables with the same content are
not `==`. Only a table compared with itself (or with an alias of it) is `==`.
`null` is its own type and equals only `null`; it is not equal to `false` or
`0`. `<=>` returns -1, 0 or 1 for ordering, and works on strings as well as
numbers.

{{example:language/types-equality}}

## typeof, type, and classof

[type](sym:type) is an ordinary function. It can be stored in a binding and
passed to [map](sym:types.Array.map) like any other value. `typeof` is an
operator, so `let describe = typeof` does not compile; it needs an operand.
`typeof(v)` works, but the parentheses group `v`; they are not a call.

{{example:language/types-type-vs-typeof}}

The three ways to ask for a value's type:

- [type](sym:type)`(v)` is the plain engine category from the list above.
  It never consults a metamethod.
- `typeof v` is the same, unless the class of `v` defines `_typeof`. Then
  the result of that method is returned. Use `type()` when the raw category
  matters.
- [classof](sym:types.classof)`(v)` returns the class object. For a scalar
  or container it is the built-in delegate (`Integer`, `String`, ...,
  imported from `"types"`). For a script class instance it is that class.

`instanceof` only walks the script class hierarchy, so `instance instanceof`
the built-in [Instance](sym:types.Instance) class is always false. To test
whether a value is any class instance, use `type(v) == "instance"`.

{{example:language/types-typeof-classof}}
