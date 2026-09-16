---
params: [start, end]
see_also: [types.Array.resize, types.String.slice]
---

Copies the elements from `start` up to, but not including, `end`.

## Parameters

- `start` - first index to copy; `0` when left out
- `end` - index to stop before; `len()` when left out

## Return value

A new array holding the copied elements, empty if the range is empty.

## Notes

Despite the trailing `...` in the signature above, `slice` is not
variadic: it takes zero, one or two real arguments, and a third is silently
ignored rather than erroring, unlike
[findvalue](sym:types.Array.findvalue). A negative `start` or `end` counts
back from the end (`-1` is the last element), and the range is clamped into
`[0, len()]` rather than throwing when it runs past either end.

## Example

{{example:types.Array.slice}}
