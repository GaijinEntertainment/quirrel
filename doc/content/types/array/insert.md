---
params: [index, value]
see_also: [types.Array.remove, types.Array.append]
---

Inserts `value` before the element currently at `index`, shifting the rest
up by one.

## Parameters

- `index` - where to insert
- `value` - the value to insert

## Return value

This array.

## Errors

Throws `index out of range` unless `0 <= index <= len()`. Note the upper
bound is inclusive: `index == len()` inserts at the end, same as
[append](sym:types.Array.append).

## Notes

`index` is not adjusted for negative values the way
[slice](sym:types.Array.slice) and [swap](sym:types.Array.swap) adjust
theirs; a negative `index` is always out of range here.

## Example

{{example:types.Array.insert}}
