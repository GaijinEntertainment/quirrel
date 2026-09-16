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

`clone` is a keyword as well as a method name: `t.clone()` never parses,
because the compiler always reads `clone` after a dot as the start of the
unary clone operator. Call it as `t["clone"]()`, or write `clone t` instead -
the `clone` operator on a table runs this same method.

## Example

{{example:types.Table.clone}}
