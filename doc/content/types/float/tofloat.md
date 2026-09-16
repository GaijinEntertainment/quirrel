---
see_also: [types.Integer.tofloat, types.Float.tointeger, types.Bool.tofloat]
---

Returns the float itself.

## Return value

`this`, unchanged. Nothing is lost, unlike
[`types.Integer.tofloat`](sym:types.Integer.tofloat), which can round a
64-bit integer down to what a 32-bit `SQFloat` can represent.

## Notes

Takes no arguments. Provided so every numeric type answers the same
`tointeger`/`tofloat` pair without the caller checking which one it already
has.

## Example

{{example:types.Float.tofloat}}
