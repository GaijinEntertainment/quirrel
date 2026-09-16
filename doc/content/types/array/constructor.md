---
params: [size, default_value]
see_also: [array, types.Array.resize]
---

Creates an array of `size` elements, each set to `default_value`.

## Parameters

- `size` - number of elements
- `default_value` - value stored in every element; `null` when left out

## Return value

The new array.

## Errors

Throws `array size must be non-negative` when `size` is negative.

## Notes

This is the same native function as the global [array](sym:array), reached
here because calling a built-in type's class object runs its constructor:
`types.Array(3, 0)` and `array(3, 0)` do the same thing. It ignores any
existing array it might be called on; `a.constructor(2)` builds and returns
a fresh array instead of resetting `a`.

`default_value` is stored as is, not cloned per slot, so a table or array
given as the fill value ends up shared by every element; see
[array](sym:array) for that in detail.

Only `size` is required; a third real argument is accepted but silently
ignored, the same way [resize](sym:types.Array.resize) ignores one.

## Example

{{example:types.Array.constructor}}
