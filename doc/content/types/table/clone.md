---
see_also: [types.Table.is_frozen, types.Table.weakref, types.Table.replace_with]
---

Returns a shallow copy of the table.

## Return value

A new table with the same keys and values. Nested tables and arrays are
shared with the original, not copied again.

## Notes

Takes no arguments. The clone is always a plain mutable table, even when the
original was frozen with `freeze()`: cloning drops the immutable flag rather
than copying it.

`clone` is a keyword as well as a method name, so by default `t.clone()` and
`t.$clone()` do not parse: the compiler reads `clone` after a dot as the
keyword. Call it as `t["clone"]()`, or write `clone t` instead. The `clone`
operator on a table runs this same method. With
[`#forbid-clone-operator`](page:language/directives#delete-and-clone) the
word is an ordinary identifier and `t.$clone()` compiles.

## Example

{{example:types.Table.clone}}
