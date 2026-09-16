---
params: [key, value]
see_also: [types.Table.rawget, types.Table.rawdelete, types.Table.rawin]
---

Sets the slot at `key` to `value`, creating the slot if it does not exist yet.

## Parameters

- `key` - the key to write
- `value` - the value to store there

## Return value

The table itself.

## Errors

Throws `Cannot modify immutable object` when the table was frozen with
`freeze()`.

## Notes

Takes exactly two arguments; `t.rawset("k")` and `t.rawset("k", 1, 2)` both
throw a wrong-number-of-parameters error.

Unlike the assignment operator `t.k = value`, which requires the slot to
already exist (use `t.k <- value` to create one), `rawset` always creates a
missing slot - there is no separate "raw newslot" method, `rawset` covers
both cases. It also never calls a `_set` or `_newslot` metamethod, for the
same reason [`rawget`](sym:types.Table.rawget) never calls `_get`: a table's
metamethods are only reachable through a delegate, which script code cannot
attach.

## Example

{{example:types.Table.rawset}}
