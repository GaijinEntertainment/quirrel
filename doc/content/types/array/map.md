---
params: [callback]
see_also: [types.Array.apply, types.Array.filter]
---

Builds a new array from the results of `callback` on each element.

## Parameters

- `callback` - `callback(value, [index], [array])`, called for each element,
  in order

## Return value

A new array, one result per source element that was not dropped (see
Notes).

## Errors

Whatever `callback` throws, on the first element where it throws, except a
thrown `null` (see Notes).

## Notes

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more.

`callback` may `throw null` to drop that element instead of contributing to
the result, so a filter and a map that can share one pass over the array
can be written as one `map` instead of `filter` followed by `map`. This is
specific to `map`: none of the other array iteration methods
([each](sym:types.Array.each), [filter](sym:types.Array.filter),
[apply](sym:types.Array.apply), [findindex](sym:types.Array.findindex),
[findvalue](sym:types.Array.findvalue), [reduce](sym:types.Array.reduce))
give a thrown `null` any special meaning; there, it aborts like any other
thrown value.

## Example

{{example:types.Array.map}}
