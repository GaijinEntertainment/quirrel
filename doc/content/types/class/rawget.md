---
params: [key]
see_also: [types.Class.rawin, types.Class.rawset, types.Class.hasindex, types.Instance.rawget]
---

Reads the value stored at `key` in the class's own member table.

## Parameters

- `key` - the member name to look up

## Return value

The value stored at `key`: a field's current default value, or the closure
stored for a method or static member.

## Errors

Throws `the index doesn't exist` when `key` is not a member of the class; check
with `rawin` first to avoid the throw.

## Notes

A class has no delegate of its own to bypass - only instances and tables do -
so `rawget` behaves like plain indexing here. Inherited members are
visible too: a derived class copies every member of its base into its own
table when it is declared, so `rawget` on the derived class reads them
directly, with no lookup chain involved.

## Example

{{example:types.Class.rawget}}
