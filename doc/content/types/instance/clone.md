---
see_also: [types.Instance.getclass, types.Class.clone, types.Class.instance]
---

Creates a new instance of the same class, copying this instance's own field
values into it.

## Return value

A new instance. Its fields hold shallow copies of this instance's field
values: a field holding an array or table is shared, not deep-cloned, with the
original.

## Notes

Does not run `constructor`. If the class defines a `_cloned` metamethod, it
runs once afterward as `newInstance._cloned(oldInstance)`. The new instance is
`this`, the original is the only explicit argument. Use `_cloned` to
deep-copy a field that must not be shared, or to fix anything a plain
field-by-field copy gets wrong.

`clone` is a compiler keyword by default (the `clone x` operator), so
`someInstance.clone()` does not even parse unless the file starts with the
`#forbid-clone-operator` directive.

## Example

{{example:types.Instance.clone}}
