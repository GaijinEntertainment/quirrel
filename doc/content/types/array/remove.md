---
params: [index]
see_also: [types.Array.insert, types.Array.pop]
---

Removes the element at `index`, shifting the rest down by one.

## Parameters

- `index` - which element to remove

## Return value

The removed element, not the array.

## Errors

Throws `index out of range` unless `0 <= index < len()`.

## Notes

`index` is not adjusted for negative values the way
[slice](sym:types.Array.slice) and [swap](sym:types.Array.swap) adjust
theirs; a negative `index` is always out of range here, same as for
[insert](sym:types.Array.insert).

## Example

{{example:types.Array.remove}}
