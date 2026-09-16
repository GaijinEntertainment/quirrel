---
see_also: [types.Table.values, types.Table.topairs, types.Table.len]
---

Returns an array of every key in the table.

## Return value

A new `array` holding one entry per slot, each the key of that slot.

## Notes

Takes no arguments. The order matches the table's current iteration order,
which the VM does not guarantee to be stable across runs or seeds - sort the
result when the order itself matters, for example before printing it.

`keys().len() == t.len()` always holds, and calling `keys()` and `values()`
back to back on the same unchanged table gives corresponding order:
`keys()[i]` is the key for `values()[i]`.

## Example

{{example:types.Table.keys}}
