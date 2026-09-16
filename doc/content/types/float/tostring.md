---
see_also: [types.Integer.tostring, types.Bool.tostring, types.Float.tointeger]
---

Converts the float to its string form.

## Return value

The value formatted through `%g` with 6 significant digits, the same
formatting `print` uses for a float. A whole-numbered float prints with no
decimal point: `(3.0).tostring()` gives `"3"`, not `"3.0"`, so on its
own the string cannot tell a whole float from an integer of the same value -
use [`type`](sym:type) when that distinction matters.

## Notes

Takes no arguments; see [`types.Integer.tostring`](sym:types.Integer.tostring)
for the shared arity error. The `%g` limit of 6 significant digits means a
value like `1.0 / 3.0` prints as `"0.333333"`, not with the full precision a
`float` could otherwise carry.

## Example

{{example:types.Float.tostring}}
