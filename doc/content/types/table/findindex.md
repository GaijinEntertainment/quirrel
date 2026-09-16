---
params: [callback]
see_also: [types.Table.findvalue, types.Table.each, types.Table.hasvalue]
---

Returns the key of the first value for which `callback` returns a truthy
result.

## Parameters

- `callback` - `callback(value, [key], [table])`, tested against every value

## Return value

The key of the first match, or nothing (`null`) if no value matches.

## Errors

Whatever `callback` throws propagates out of `findindex`.

## Notes

Takes exactly one argument - `callback` - despite the VM-reported signature
showing no brackets or `...`; there is no optional second argument here (see
[`findvalue`](sym:types.Table.findvalue), which does have one and is easy to
confuse this with).

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more - the same rule as
[`each`](sym:types.Table.each).

Iteration order is otherwise unspecified, so `callback` should not rely on
seeing keys in any particular order, only that every slot is offered once
until a match is found.

## Example

{{example:types.Table.findindex}}
