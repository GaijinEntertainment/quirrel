---
params: [key]
see_also: [types.Table.rawget, types.Table.hasindex, types.Table.hasvalue]
---

Reports whether `key` is a slot in the table.

## Parameters

- `key` - the key to test

## Return value

`true` if `key` names a slot in the table, `false` otherwise.

## Notes

Takes exactly one argument; `t.rawin()` throws a wrong-number-of-parameters
error.

Same result as [`hasindex`](sym:types.Table.hasindex) on a table: both go
through `sq_rawget`. The `in` operator, `key in t`, also agrees with `rawin` on
a table, because a table has no `_get` metamethod.

## Example

{{example:types.Table.rawin}}
