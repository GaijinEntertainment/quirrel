---
see_also: [types.Table.clone, types.Table.is_frozen, types.Table.weakref]
---

Returns a string that names the type and identity of the table.

## Return value

A `string` shaped like `(table : 0x...)`. The address is the table's own
identity, not its contents: two tables with the same keys and values print two
different addresses, and the same table prints the same address every time.

## Notes

Takes no arguments; `t.tostring(1)` throws `wrong number of parameters passed
to native closure 'tostring' (2 passed, 1 required)`.

Because the address changes from run to run, code must never compare or print
`tostring()` of a table for anything other than a human to eyeball while
debugging.

## Example

{{example:types.Table.tostring}}
