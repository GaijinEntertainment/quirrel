---
see_also: [types.Thread.weakref, types.Function.clone]
---

Returns the thread itself: a thread has no separate clone identity.

## Return value

The same thread `clone` was called on - `clone t == t` is always true,
whatever state `t` is in.

## Notes

`clone` is a keyword as well as a method name, so by default `t.clone()` and
`t.$clone()` do not parse. Use the bracket form, `t["clone"]()`, or the
operator form, `clone t`. Both reach this same method. With
`#forbid-clone-operator` the word is an ordinary identifier and `t.$clone()`
compiles.

## Example

{{example:types.Thread.clone}}
