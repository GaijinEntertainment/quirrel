---
see_also: [types.Integer.tochar, types.Bool.tochar, types.Float.tointeger]
---

Converts the float to a one-character string, treating it as a character
code.

## Return value

A `string` of length 1. The float is truncated toward zero first (the same
truncation [`tointeger`](sym:types.Float.tointeger) does), then cast to
`char`, so the fractional part never affects the result:
`(65.0).tochar()` and `(65.9).tochar()` both give `"A"`.

## Notes

Takes no arguments. See
[`types.Integer.tochar`](sym:types.Integer.tochar) for the modulo-256
wraparound, which applies here too once the value is truncated.

## Example

{{example:types.Float.tochar}}
