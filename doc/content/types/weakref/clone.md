---
see_also: [types.WeakRef.ref, types.Table.clone, types.Integer.clone]
---

Returns this `weakref`.

## Return value

A value `==` to `this`. Cloning an atomic reference type such as `weakref`
has nothing to copy piece by piece, unlike
[`types.Table.clone`](sym:types.Table.clone), which builds a genuinely new
table.

## Notes

Takes no arguments. `clone` is a keyword, so `wr.clone()` never parses;
call it through a computed index, `wr["clone"]()`, or write `clone wr`
instead - see [`types.Integer.clone`](sym:types.Integer.clone).

## Example

{{example:types.WeakRef.clone}}
