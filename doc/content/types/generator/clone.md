---
see_also: [types.Generator.weakref, types.Function.clone]
---

Returns the generator itself: a generator has no separate clone identity.

## Return value

The same generator `clone` was called on - `clone g == g` is always true,
whatever state `g` is in.

## Notes

`clone` is a keyword as well as a method name, so by default `g.clone()` and
`g.$clone()` do not parse. Use the bracket form, `g["clone"]()`, or the
operator form, `clone g`. Both reach this same method. With
`#forbid-clone-operator` the word is an ordinary identifier and `g.$clone()`
compiles.

## Example

{{example:types.Generator.clone}}
