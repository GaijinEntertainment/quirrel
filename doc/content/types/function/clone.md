---
see_also: [types.Function.weakref, types.Function.bindenv]
---

Returns the closure itself: unlike a table, array or class instance, a
closure has no separate clone identity.

## Return value

The same closure `clone` was called on - `clone f == f` is always true.

## Notes

`clone` is a keyword as well as a method name, so calling it through `.`
needs the bracket form, `f["clone"]()`, wherever the parser would otherwise
read `clone` as the unary clone operator; the operator form, `clone f`, works
everywhere. Both reach this same method.

Cloning a closure to attach a different environment is a different
operation, done with `bindenv`, which does return a new closure.

## Example

{{example:types.Function.clone}}
