---
see_also: [types.Table.keys, types.Table.values, types.Table.rawin]
---

Returns the number of key/value pairs in the table.

## Return value

A non-negative `int`: the number of slots currently in the table.

## Notes

Takes no arguments. `t.len()` and `t.len(1)` differ only in that the second
throws `wrong number of parameters passed to native closure 'len' (2 passed,
1 required)` - the VM checks the count before `len` ever runs.

## Example

{{example:types.Table.len}}
