---
see_also: [types.Array.replace_with, freeze, types.Array.is_frozen]
---

Makes a shallow copy of the array.

## Return value

A new array with the same elements.

## Notes

`clone` is a language keyword (the `clone` operator), so `a.clone()` and
`a.$clone()` do not compile: the parser expects an identifier after `.` and
rejects the keyword. Write `clone a` instead, or, if the call has to be built
from a string, `a["clone"]()`. With
[`#forbid-clone-operator`](page:language/directives#delete-and-clone) the
word is an ordinary identifier and `a.$clone()` compiles.

The copy is shallow: an element that is itself a table, array, class or
instance is not copied, so the original and the clone reach the same nested
object, and a change through one is visible through the other. Only the
top-level list of elements is independent.

A frozen array clones into a plain, unfrozen one; freezing is not part of
what gets copied, see [is_frozen](sym:types.Array.is_frozen).

## Example

{{example:types.Array.clone}}
