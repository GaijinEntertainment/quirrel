---
params: [other]
see_also: [types.Array.clone, types.Array.extend]
---

Overwrites this array's elements with `other`'s.

## Parameters

- `other` - the array to copy elements from

## Return value

This array.

## Notes

The new length matches `other`, so this can grow or shrink the array,
unlike [apply](sym:types.Array.apply). The elements themselves are copied
by reference, the same as [slice](sym:types.Array.slice) and
[clone](sym:types.Array.clone): a table or array element ends up shared
between this array and `other` afterward.

[replace](sym:types.Array.replace) is a deprecated alias for this function.

## Example

{{example:types.Array.replace_with}}
