---
params: [key]
see_also: [types.Table.rawset, types.Table.clear, types.Table.rawin]
---

Removes the slot at `key`, without calling a `_delslot` metamethod.

## Parameters

- `key` - the key to remove

## Return value

The value that was stored at `key`, or `null` if the key was not present.

## Errors

Throws `Cannot modify immutable object` when the table was frozen with
`freeze()`.

## Notes

Write it as `t.$rawdelete(key)`. A table's own slots and its methods share one
namespace, so a table that happens to have a slot called `rawdelete` hides the
method behind it, and `t.rawdelete("a")` then fails with `attempt to call
'string'`. The `$` prefix reaches the built-in type method directly and never
sees the slot. Plain `t.rawdelete(key)` works whenever no such slot exists, but
on data whose keys you do not control, `$` always works.

Takes exactly one argument; `t.$rawdelete()` and `t.$rawdelete("a", "b")` both
throw a wrong-number-of-parameters error.

A missing key is not an error here, unlike [`rawget`](sym:types.Table.rawget):
deleting a key that is already absent is a silent no-op that returns `null`.

The `delete t.k` operator is forbidden by default (it needs the
`#allow-delete-operator` pragma); the compiler's own error for it points at
this method: "Usage of 'delete' operator is forbidden. Use
'o.$rawdelete(\"key\")' instead".

## Example

{{example:types.Table.rawdelete}}
