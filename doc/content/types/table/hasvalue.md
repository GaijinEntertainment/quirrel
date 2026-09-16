---
params: [value]
see_also: [types.Table.hasindex, types.Table.findvalue, types.Table.rawin]
---

Reports whether `value` is stored under any key in the table.

## Parameters

- `value` - the value to search for

## Return value

`true` if some key maps to a value equal to `value`, `false` otherwise.

## Notes

Takes exactly one argument; `t.hasvalue()` throws a wrong-number-of-parameters
error.

Walks every slot in the table's current iteration order and stops at the
first match, so the answer does not depend on that order - only whether a
match exists does. Equality is raw (like `<=>`'s fallback, never a `_cmp`
metamethod): numbers compare by value, and tables, arrays and instances
compare by identity, so a different table with equal contents is not a
match.

Use [`findvalue`](sym:types.Table.findvalue) instead when the match itself,
or its key, is needed rather than only a yes/no answer.

## Example

{{example:types.Table.hasvalue}}
