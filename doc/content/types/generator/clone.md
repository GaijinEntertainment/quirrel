---
see_also: [types.Generator.weakref, types.Function.clone]
---

Returns the generator itself: a generator has no separate clone identity.

## Return value

The same generator `clone` was called on - `clone g == g` is always true,
whatever state `g` is in.

## Notes

`clone` is a keyword as well as a method name, so calling it through `.`
needs the bracket form, `g["clone"]()`, wherever the parser would otherwise
read `clone` as the unary clone operator; the operator form, `clone g`, works
everywhere. Both reach this same method.

## Example

{{example:types.Generator.clone}}
