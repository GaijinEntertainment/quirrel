---
see_also: [types.Table.keys, types.Table.topairs, types.Table.len]
---

Returns an array of every value in the table.

## Return value

A new `array` holding one entry per slot, each the value of that slot.

## Notes

Takes no arguments. The order matches the table's current iteration order,
which the VM does not guarantee to be stable across runs or seeds - sort or
otherwise post-process the result when the order itself matters.

`values().len() == t.len()` always holds, and calling
[`keys`](sym:types.Table.keys) and `values` back to back on the same
unchanged table gives corresponding order.

## Example

{{example:types.Table.values}}
