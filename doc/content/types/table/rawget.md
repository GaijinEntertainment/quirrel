---
params: [key]
see_also: [types.Table.rawset, types.Table.rawin, types.Table.hasindex]
---

Returns the value stored under `key`.

## Parameters

- `key` - the key to look up

## Return value

The value stored under `key`.

## Errors

Throws `the index doesn't exist` when `key` is not in the table.

## Notes

Takes exactly one argument - `t.rawget()` and `t.rawget("a", "b")` both throw
a wrong-number-of-parameters error rather than reaching this code.

Plain indexing, `t[key]`, and `rawget` both throw on a missing key for an
ordinary table (with different wording - `t[key]` throws
`the index 'key' (type='...') does not exist`), because a table has no `_get`
metamethod. The distinction between raw and plain access matters far more for
an instance, where `_get` is reachable from script and can turn a "missing" key
into a computed value.

## Example

{{example:types.Table.rawget}}
