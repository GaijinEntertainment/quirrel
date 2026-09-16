---
see_also: [types.WeakRef.ref, types.Table.clone, types.Integer.clone]
---

Returns this `weakref`.

## Return value

A value `==` to `this`. Cloning an atomic reference type such as `weakref`
has nothing to copy piece by piece, unlike
[`types.Table.clone`](sym:types.Table.clone), which builds a new table.

## Notes

Takes no arguments. `clone` is a keyword, so by default `wr.clone()` and
`wr.$clone()` do not parse. Call it through a computed index,
`wr["clone"]()`, or write `clone wr`. With `#forbid-clone-operator` the
word is an ordinary identifier and `wr.$clone()` compiles. See
[`types.Integer.clone`](sym:types.Integer.clone).

## Example

{{example:types.WeakRef.clone}}
