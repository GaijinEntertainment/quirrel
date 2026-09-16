---
params: [callback]
see_also: [types.Table.map, types.Table.each, types.Table.findvalue]
---

Returns a new table holding only the slots for which `callback` returns a
truthy result.

## Parameters

- `callback` - `callback(value, [key], [table])`, tested against every slot

## Return value

A new table with the same key and value as the original for every slot that
passed the test; `callback`'s return value only decides membership, it never
replaces the value the way [`map`](sym:types.Table.map)'s does.

## Errors

Whatever `callback` throws propagates out of `filter` - unlike `map`,
`filter` has no special case for `throw null`; any thrown value, including
`null`, aborts the call.

## Notes

Takes exactly one argument, `callback`.

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more - the same rule as
[`each`](sym:types.Table.each).

The result is always a new table; the original is never modified.

## Example

{{example:types.Table.filter}}
