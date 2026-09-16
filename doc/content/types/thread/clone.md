---
see_also: [types.Thread.weakref, types.Function.clone]
---

Returns the thread itself: a thread has no separate clone identity.

## Return value

The same thread `clone` was called on - `clone t == t` is always true,
whatever state `t` is in.

## Notes

`clone` is a keyword as well as a method name, so calling it through `.`
needs the bracket form, `t["clone"]()`, wherever the parser would otherwise
read `clone` as the unary clone operator; the operator form, `clone t`, works
everywhere. Both reach this same method.

## Example

{{example:types.Thread.clone}}
