---
see_also: [types.WeakRef.ref, types.WeakRef.constructor, types.Integer.weakref]
---

Returns a weak reference to this `weakref`.

## Return value

A new `weakref`, distinct from `this`, whose `ref()` gives back `this` (the
original `weakref` object) for as long as something else still holds a
strong reference to it - not the value `this` itself points at.

## Notes

A `weakref` object is reference counted, unlike the scalar types where this
same method is a no-op; see
[`types.Integer.weakref`](sym:types.Integer.weakref) for that contrast and
for the meaning of the shown receiver, `(table|userdata|instance|class|null)`.
That list does not restrict the valid types - the method accepts any of
them, `weakref` included.

## Example

{{example:types.WeakRef.weakref}}
