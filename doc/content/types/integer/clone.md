---
see_also: [types.Float.clone, types.Bool.clone, types.Table.clone, types.Integer.weakref]
---

Returns the integer itself.

## Return value

`this`, unchanged. An integer has no identity separate from its value, so
there is nothing a copy could do differently from returning the same value -
unlike [`types.Table.clone`](sym:types.Table.clone), which allocates a new
table.

## Notes

Takes no arguments. `clone` is a keyword as well as a method name, so
`(5).clone()` never parses: the compiler always reads `clone` after a dot as
the start of the unary `clone` operator, not a field name. Write `clone 5`,
or call through a computed index, `x["clone"]()`, to reach the method
directly; see [`types.Table.clone`](sym:types.Table.clone) for why the dot
form is blocked at all.

## Example

{{example:types.Integer.clone}}
