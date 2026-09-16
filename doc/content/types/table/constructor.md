---
see_also: [types.Table.clone, types.Table.clear]
---

Returns a new, empty table.

## Return value

A fresh table with no keys, unrelated to the table `constructor` was called
on.

## Notes

Takes no arguments. Unlike a class's `constructor`, calling this one on an
existing table `t` does not rebuild `t` in place: `t.constructor()` leaves
`t` untouched and returns a different, empty table. Use
[`clear`](sym:types.Table.clear) to empty `t` itself, or the `{}` literal to
make a new empty table directly; this method exists mainly so the table
delegate has a `constructor` slot, matching every other type.

## Example

{{example:types.Table.constructor}}
