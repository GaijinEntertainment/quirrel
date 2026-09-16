---
params: [callback]
see_also: [types.Array.findvalue, types.Array.indexof]
---

Finds the index of the first element for which `callback` returns a true
value.

## Parameters

- `callback` - `callback(value, [index], [array])`, tested against each
  element, in order

## Return value

The index of the first match, or `null` if none matched.

## Errors

Whatever `callback` throws, on the first element where it throws.

## Notes

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more.

Use [indexof](sym:types.Array.indexof) instead when the target is a plain
value to compare, rather than a condition to test.

## Example

{{example:types.Array.findindex}}
