---
params: [value]
see_also: [types.Array.contains, types.Array.findindex]
---

Finds the index of the first element that equals `value`.

## Parameters

- `value` - the value to look for

## Return value

The index of the first match, or `null` if none matched.

## Notes

Equality is the same raw comparison [contains](sym:types.Array.contains)
uses: identity for tables, arrays, classes and instances, value for numbers
and strings, and no `_cmp` or `_eq` metamethod. Use
[findindex](sym:types.Array.findindex) when the match is a condition rather
than a fixed value.

## Example

{{example:types.Array.indexof}}
