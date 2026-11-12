---
see_also: [deep.copy]
---

Copies the object graph of `value`: its tables, its arrays and the instances
whose class has a `_deepclone(cloner)` method. It is for the values that `copy`
cannot copy: a cycle, a subtree that must stay shared, and an instance, which
`copy` shares.

## Parameters

- `value` - the value to copy

## Return value

The copy.

## Errors

Throws `copy_graph: the value is nested deeper than 1000 levels` for tables and
arrays nested deeper than that between two `_deepclone` calls, and
`Native stack overflow` at about 50 nested `_deepclone` methods that call
`cloner`. An error in a `_deepclone` method goes to the caller, also from
`throw null`.

## Notes

- Each table, array and copied instance is copied one time, so a subtree that
  two places share stays shared in the copy, and a cycle is copied as a cycle;
  table keys are not copied.
- An instance whose class has a `_deepclone(cloner)` method is copied by that
  method. `cloner` copies a value with the same rules and the same record of
  copies, so the method can copy its fields with it. An instance without
  `_deepclone` stays shared, and so do classes and functions.
- A weakref stays a weakref, and the copies are not frozen.

A cycle through an instance with `_deepclone` is not supported, because the
method runs again for the same instance before it returns.

## Example

{{example:deep.copy_graph}}
