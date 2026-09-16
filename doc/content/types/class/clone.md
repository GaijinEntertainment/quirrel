---
see_also: [types.Class.instance, types.Class.call, types.Instance.clone]
---

Always fails. `clone` is one of the type-methods every built-in delegate
carries, but the VM's generic clone operation explicitly refuses a class.

## Return value

Never returns.

## Errors

Always throws `cloning a class`.

## Notes

Cloning is defined for tables, arrays, and instances (see
`types.Instance.clone`), each of which copies its own stored values into a
fresh object. A class is a shared *definition*, not a bag of instance data, so
there is nothing meaningful for clone to copy; the same restriction applies to
`userdata`. To get a new, independent class, use `instance()` to make an
instance instead, or `__merge` to build a new class out of this one's members.

`clone` is a compiler keyword by default (the `clone x` operator), so
`SomeClass.clone()` does not even parse unless the file starts with the
`#forbid-clone-operator` directive; every example on this page and on
`types.Instance.clone` carries it for that reason.

## Example

{{example:types.Class.clone}}
