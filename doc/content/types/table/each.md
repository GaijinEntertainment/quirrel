---
params: [callback]
see_also: [types.Table.map, types.Table.filter, types.Table.reduce]
---

Calls `callback` once for every slot in the table.

## Parameters

- `callback` - `callback(value, [key], [table])`, called for every slot

## Return value

Nothing (`null`); `each` is used for its side effects.

## Errors

Whatever `callback` throws propagates out of `each`.

## Notes

Takes exactly one argument, `callback`.

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more; one that declares none is called once per slot
with no arguments. Use this to count slots or to repeat a side effect.

Iteration order is whatever the table's current order happens to be, which
the VM does not guarantee to be stable across runs or seeds - do not let
`callback`'s side effects depend on the order slots are visited in.

## Example

{{example:types.Table.each}}
