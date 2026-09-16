---
see_also: [freeze, types.Array.resize]
---

Creates an array of `size` elements, each set to `default_value`.

## Parameters

- `size` - number of elements
- `default_value` - value stored in every element

## Return value

The new array.

## Errors

Throws `array size must be non-negative` when `size` is negative.

## Notes

`default_value` fills every element as is, not a fresh copy per element: if it
is a table or array, every element aliases the same object, and a change
through one element shows up in all of them. Fill the array in a loop instead
when the elements must be independent.

When `default_value` is left out, every element is `null`.

## Example

{{example:array}}
