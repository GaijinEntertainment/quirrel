---
params: [callback]
see_also: [types.Array.map, types.Array.reduce]
---

Calls `callback` once per element, in order.

## Parameters

- `callback` - `callback(value, [index], [array])`, called for each element

## Return value

Nothing (`null`); `each` is for the side effect, not for a result.

## Errors

Whatever `callback` throws, on the first element where it throws; iteration
stops there.

## Notes

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more.

## Example

{{example:types.Array.each}}
