---
see_also: [types.Integer.clone, types.Float.clone, types.Bool.weakref]
---

Returns the bool itself.

## Return value

`this`, unchanged, for the same reason as
[`types.Integer.clone`](sym:types.Integer.clone).

## Notes

Takes no arguments. `clone` is a keyword, so `true.clone()` never parses; see
[`types.Integer.clone`](sym:types.Integer.clone) for the operator and
computed-index alternatives.

## Example

{{example:types.Bool.clone}}
