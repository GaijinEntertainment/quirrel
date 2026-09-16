---
see_also: [types.Array.resize, types.Array.clone]
---

Removes every element from the array.

## Return value

This array, now empty.

## Notes

Equivalent to `resize(0)`, including that the backing storage is released
once the array shrinks past its shrink threshold, not merely marked empty.

## Example

{{example:types.Array.clear}}
