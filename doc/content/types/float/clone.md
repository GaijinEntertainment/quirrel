---
see_also: [types.Integer.clone, types.Bool.clone, types.Float.weakref]
---

Returns the float itself.

## Return value

`this`, unchanged, for the same reason as
[`types.Integer.clone`](sym:types.Integer.clone): a float has no identity
apart from its value.

## Notes

Takes no arguments. `clone` is a keyword, so `(3.5).clone()` never parses;
see [`types.Integer.clone`](sym:types.Integer.clone) for the operator and
computed-index alternatives.

## Example

{{example:types.Float.clone}}
