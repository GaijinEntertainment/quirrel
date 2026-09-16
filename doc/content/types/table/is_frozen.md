---
see_also: [types.Table.clone, types.Table.weakref, types.Table.clear]
---

Reports whether this reference to the table was frozen with `freeze()`.

## Return value

`true` if the table is immutable, `false` otherwise.

## Notes

Takes no arguments. Checks the table itself, not what it contains: freezing a
table freezes only its own slots, so `is_frozen()` on a table holding a
mutable nested table still reports `true` for the outer one while the inner
one stays freely editable.

[`clone`](sym:types.Table.clone) never copies the flag: cloning a frozen
table gives back a table for which `is_frozen()` is `false`.

## Example

{{example:types.Table.is_frozen}}
