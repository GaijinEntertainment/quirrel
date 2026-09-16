---
see_also: [types.Integer.weakref, types.Bool.weakref, types.Float.clone]
---

Returns the float itself.

## Return value

`this`, unchanged - not a `weakref` value.

## Notes

A `float` is never reference counted, so there is nothing for `sq_weakref`
to point a real `weakref` at; see
[`types.Integer.weakref`](sym:types.Integer.weakref) for the full mechanism,
including why the signature's shown receiver,
`(table|userdata|instance|class|null)`, does not name `float`
either.

## Example

{{example:types.Float.weakref}}
