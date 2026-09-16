---
see_also: [types.Bool.tointeger, types.Integer.constructor, types.Float.constructor]
---

Converts `value` to a bool. This is what runs when `types.Bool` itself is
called, `types.Bool(value)`.

## Return value

`false` when called with no arguments. Otherwise the truthiness of `value`,
using the same rule as `if` and `!`: only `null`, `false`, the integer `0`
and the float `0.0` are false. Everything else is true, including an empty
string and an empty array or table - `types.Bool("")` and `types.Bool([])`
both give `true`, unlike languages where an empty container or string counts
as false.

## Notes

Never throws: every type has a truth value, so this always succeeds. Takes
zero or more arguments; a second and later argument is accepted and ignored,
the same generic fallback receiver described on
[`types.String.constructor`](sym:types.String.constructor) shows up here too
since the binding carries no declaration string.

## Example

{{example:types.Bool.constructor}}
