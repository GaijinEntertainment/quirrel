---
params: [callback]
see_also: [types.Table.filter, types.Table.each, types.Table.reduce]
---

Returns a new table with the same keys, each mapped through `callback`.

## Parameters

- `callback` - `callback(value, [key], [table])`, called for every slot; its
  return value becomes the new value for that key

## Return value

A new table, same keys as the original, each value replaced by
`callback`'s return value for that slot.

## Errors

Whatever `callback` throws propagates out of `map`, with one exception: a
`callback` that does `throw null` for a given slot is not an error here -
that slot is silently left out of the result table instead of being mapped.
Throwing anything else still aborts the call.

## Notes

Takes exactly one argument, `callback`.

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more - the same rule as
[`each`](sym:types.Table.each); `table` is the original, before any slot is
replaced.

The result is always a new table; the original is never modified.

## Example

{{example:types.Table.map}}
