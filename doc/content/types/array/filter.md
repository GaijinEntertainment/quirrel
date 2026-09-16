---
params: [callback]
see_also: [types.Array.map, types.Array.findvalue]
---

Collects the elements for which `callback` returns a true value.

## Parameters

- `callback` - `callback(value, [index], [array])`, tested against each
  element, in order

## Return value

A new array of the elements that passed.

## Errors

Whatever `callback` throws, on the first element where it throws.

## Notes

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more.

## Example

{{example:types.Array.filter}}
