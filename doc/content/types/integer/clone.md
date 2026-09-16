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

Takes no arguments. `clone` is a keyword as well as a method name, so by
default `(5).clone()` and `(5).$clone()` do not parse: the compiler reads
`clone` after a dot as the keyword, not a field name. Write `clone 5`, or
call through a computed index, `x["clone"]()`, to reach the method directly.
With [`#forbid-clone-operator`](page:language/directives#delete-and-clone)
the word is an ordinary identifier and `x.$clone()` compiles. See
[`types.Table.clone`](sym:types.Table.clone).

## Example

{{example:types.Integer.clone}}
