---
see_also: [types.Integer.tostring, types.Float.tochar, types.Bool.tochar]
---

Converts the integer to a one-character string, treating it as a character
code.

## Return value

A `string` of length 1 whose only byte is `this` cast to `char`.

## Notes

Takes no arguments. The cast wraps modulo 256 and keeps no sign information,
the same as a C `(char)` cast: `(65).tochar()` and `(65 + 256).tochar()` give
the same character. A negative or out-of-`[0, 255]` value does not throw, it
wraps.

This method also exists on `float` and `bool` (see
[`types.Float.tochar`](sym:types.Float.tochar) and
[`types.Bool.tochar`](sym:types.Bool.tochar)); `bool`'s version produces a
control character rather than a printable one, since `true` casts to `1`.

## Example

{{example:types.Integer.tochar}}
