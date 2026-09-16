---
see_also: [types.Table.is_frozen, types.Table.clone, types.Table.tostring]
---

Returns a weak reference to the table.

## Return value

A `weakref` value. Calling its `ref()` method gives the table back for as
long as something else still holds a strong reference to it; once the
table's refcount drops to zero, `ref()` returns `null` instead.

## Notes

Takes no arguments. Holding a `weakref` never keeps the table alive by
itself. Use one instead of an ordinary reference to observe an object without
extending its lifetime.

Storing the `weakref` itself in a container slot is different from holding it
in a local: reading that slot back unwraps it and returns the table (or
`null` once the table is gone), not the `weakref` value.

## Example

{{example:types.Table.weakref}}
