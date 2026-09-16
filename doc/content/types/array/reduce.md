---
params: [callback, initial]
see_also: [types.Array.map, types.Array.each]
---

Folds the array down to one value by repeatedly combining an accumulator
with each element.

## Parameters

- `callback` - `callback(accumulator, value, [index], [array])`, combines the
  accumulator with the next element
- `initial` - the accumulator's starting value; the first element when left
  out

## Return value

The final accumulator. On an empty array: `initial` if it was given,
otherwise `null`, either way without calling `callback`.

## Errors

Whatever `callback` throws, on the first element where it throws.

## Notes

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more. `index` and `array` count the element
supplying `value`, which starts at
index `1` when `initial` was left out, since element `0` seeded the
accumulator instead of being folded in.

When `initial` is left out and the array has exactly one element, that
element is returned as is and `callback` is never called.

## Example

{{example:types.Array.reduce}}
