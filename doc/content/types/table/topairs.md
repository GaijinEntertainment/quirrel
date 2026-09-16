---
see_also: [types.Table.keys, types.Table.values, types.Table.each]
---

Returns an array of `[key, value]` pairs, one per slot in the table.

## Return value

A new `array` of 2-element arrays; each inner array is `[key, value]` for one
slot.

## Notes

Takes no arguments. The order matches the table's current iteration order,
which the VM does not guarantee to be stable across runs or seeds - sort the
result (for example by the key at index 0) before printing or comparing it.

Equivalent to zipping [`keys`](sym:types.Table.keys) and
[`values`](sym:types.Table.values) together, but in one walk of the table
instead of two.

## Example

{{example:types.Table.topairs}}
