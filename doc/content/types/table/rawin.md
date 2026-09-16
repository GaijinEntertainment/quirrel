---
params: [key]
see_also: [types.Table.rawget, types.Table.hasindex, types.Table.hasvalue]
---

Reports whether `key` is a slot in the table, without calling a metamethod.

## Parameters

- `key` - the key to test

## Return value

`true` if `key` names a slot in the table, `false` otherwise.

## Notes

Takes exactly one argument; `t.rawin()` throws a wrong-number-of-parameters
error.

Same result as [`hasindex`](sym:types.Table.hasindex) on a table: both go
through `sq_rawget` and never consult a `_get` metamethod. The `in`
operator, `key in t`, is close but technically not raw - it still runs the
metamethod-aware lookup - yet for a table it always agrees with `rawin`
anyway, because a table's `_get` can only come from a delegate, and script
code has no way to attach one (see
[`getfuncinfos`](sym:types.Table.getfuncinfos)).

## Example

{{example:types.Table.rawin}}
