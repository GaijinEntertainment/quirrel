---
see_also: [types.Integer.weakref, types.Float.weakref, types.Bool.clone]
---

Returns the bool itself.

## Return value

`this`, unchanged - not a `weakref` value.

## Notes

A `bool` is never reference counted, so there is nothing for `sq_weakref` to
point a real `weakref` at; see
[`types.Integer.weakref`](sym:types.Integer.weakref) for the full mechanism.

## Example

{{example:types.Bool.weakref}}
