---
params: [comparator]
see_also: [types.Array.reverse]
---

Sorts the array in place.

## Parameters

- `comparator` - compares two elements; the default ordering when left out
  (see Notes)

## Return value

This array.

## Errors

Whatever `comparator` throws. Without a `comparator`, throws `comparison
between X and Y` for a pair of elements neither numeric-with-numeric,
string-with-string, nor both instances of a class (or tables) that defines
`_cmp`.

## Notes

`comparator(a, b)` must return a negative number when `a` sorts before `b`,
a positive number when it sorts after, and `0` when they are equal, the same
convention as `_cmp`.

Despite the trailing `...` in the signature above, `sort` is not variadic:
it takes zero or one real argument. Calling it with more than one extra
argument does not throw, unlike [findvalue](sym:types.Array.findvalue); it
silently falls back to the default ordering, as if `comparator` had been
left out entirely.

Without a `comparator`, elements are compared the way `<` compares them:
numbers by value, strings lexicographically, and a table or instance through
its `_cmp` metamethod if it has one; anything else is a `comparison between`
error.

`sort` is not a stable sort: elements that compare equal can still change
their order relative to each other. Do not rely on the relative order of
elements a `comparator` reports as equal.

## Example

{{example:types.Array.sort}}
