---
params: [index1, index2]
see_also: [types.Array.insert, types.Array.remove]
---

Swaps the elements at `index1` and `index2`.

## Parameters

- `index1` - index of the first element
- `index2` - index of the second element

## Return value

This array.

## Errors

Throws `index out of range` unless both indices, after the adjustment
described in Notes, are within `[0, len())`.

## Notes

Unlike [insert](sym:types.Array.insert) and
[remove](sym:types.Array.remove), a negative index here counts back from
the end, the same way [slice](sym:types.Array.slice) counts its bounds:
`-1` is the last element.

## Example

{{example:types.Array.swap}}
