---
see_also: [types.Integer.tostring, types.Float.tostring, types.Bool.tointeger]
---

Converts the bool to its string form.

## Return value

The literal word `"true"` or `"false"` - not `"1"`/`"0"`, unlike
[`tointeger`](sym:types.Bool.tointeger).

## Notes

Takes no arguments; see
[`types.Integer.tostring`](sym:types.Integer.tostring) for the shared arity
error. `null` has no `tostring` method to compare against: it is not
callable as a method, only formatted implicitly (as `"null"`) when the
VM converts it for `print` or string concatenation.

## Example

{{example:types.Bool.tostring}}
