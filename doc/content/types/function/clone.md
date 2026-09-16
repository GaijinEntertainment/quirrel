---
see_also: [types.Function.weakref, types.Function.bindenv]
---

Returns the closure itself: unlike a table, array or class instance, a
closure has no separate clone identity.

## Return value

The same closure `clone` was called on - `clone f == f` is always true.

## Notes

`clone` is a keyword as well as a method name, so by default `f.clone()` and
`f.$clone()` do not parse. Use the bracket form, `f["clone"]()`, or the
operator form, `clone f`. Both reach this same method. With
`#forbid-clone-operator` the word is an ordinary identifier and `f.$clone()`
compiles.

Cloning a closure to attach a different environment is a different
operation, done with `bindenv`, which does return a new closure.

## Example

{{example:types.Function.clone}}
