---
see_also: [math.hash, types.String.len]
---

Returns `str`'s hash.

## Return value

A non-negative `int`: the FNV-1a hash the VM computed once, when `str` was
created, and cached on the string object. Equal strings always give the same
hash.

## Notes

Takes no arguments; the VM reports the true arity here.

This is not the same value as `math.hash(str)`. `math.hash` runs its own
mixing pass, seeded independently, that folds in `str`'s cached hash together
with the string's type tag and its length - so the two functions agree on
which strings are equal (equal strings give equal results from either one)
but not on what the number is: `"x".hash() != hash("x")` in general.

## Example

{{example:types.String.hash}}
