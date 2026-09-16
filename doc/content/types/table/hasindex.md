---
params: [key]
see_also: [types.Table.hasvalue, types.Table.rawin, types.Table.rawget]
---

Reports whether `key` is a slot in the table.

## Parameters

- `key` - the key to test

## Return value

`true` if `key` names a slot in the table, `false` otherwise.

## Notes

Takes exactly one argument; `t.hasindex()` throws a wrong-number-of-parameters
error.

Implemented the same way as [`rawin`](sym:types.Table.rawin): both call
`sq_rawget` directly, so neither ever consults a `_get` metamethod. The two
exist as separate methods because the shared C++ function also backs
`class` and `instance`, where `hasindex` additionally accepts those types;
on a table the two methods behave identically.

## Example

{{example:types.Table.hasindex}}
