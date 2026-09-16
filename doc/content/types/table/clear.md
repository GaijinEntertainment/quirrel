---
see_also: [types.Table.rawdelete, types.Table.replace_with, types.Table.is_frozen]
---

Removes every key/value pair from the table, in place.

## Return value

The table itself, now empty.

## Errors

Throws `Cannot modify immutable object` when the table was frozen with
`freeze()`.

## Notes

Takes no arguments. Existing references to the table see it become empty too,
since `clear` mutates the same object rather than building a new one -
unlike `constructor()`, which returns an unrelated new table.

## Example

{{example:types.Table.clear}}
