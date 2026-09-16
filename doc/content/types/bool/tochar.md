---
see_also: [types.Integer.tochar, types.Float.tochar, types.Bool.tointeger]
---

Converts the bool to a one-character string, treating it as a character
code.

## Return value

A `string` of length 1: `chr(1)` for `true`, `chr(0)` (a NUL byte) for
`false`. Neither is printable - `true`'s casts to the same character code as
`(1).tochar()`, a control character, not a letter or digit.

## Notes

Takes no arguments. See
[`types.Integer.tochar`](sym:types.Integer.tochar) for the general
`(char)`-cast mechanism this shares.

## Example

{{example:types.Bool.tochar}}
