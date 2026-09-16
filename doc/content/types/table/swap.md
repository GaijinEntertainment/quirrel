---
params: [key1, key2]
see_also: [types.Table.rawset, types.Table.rawget, types.Table.replace_with]
---

Exchanges the values stored at `key1` and `key2`.

## Parameters

- `key1` - first key
- `key2` - second key

## Return value

The table itself.

## Errors

Throws `the index doesn't exist` if either key is not present. Throws
`Cannot modify immutable object` when the table was frozen with `freeze()`.

## Notes

Takes exactly two arguments. Both keys must already exist - `swap` never
creates a slot, unlike [`rawset`](sym:types.Table.rawset).

The same method also backs `array` and `instance`; on an array the two
arguments are treated as indices instead of keys, and negative indices count
from the end.

## Example

{{example:types.Table.swap}}
