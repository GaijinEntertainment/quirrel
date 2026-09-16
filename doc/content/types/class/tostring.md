---
see_also: [types.Instance.tostring, types.Class.getmetamethod]
---

Converts the class to a string.

## Return value

Always `"(class : 0x" + address + ")"`, the VM's generic default. A class
cannot customize its own conversion.

## Notes

A `_tostring` method defined in a class body customizes how *instances* of
that class convert to a string (see `types.Instance.tostring`); it has no
effect on the class object itself, since class-level indexing has no delegate
lookup for `class.tostring()` to fall back through. `Foo.tostring()` and
`(class)Foo.tostring()`-style string concatenation both always show the raw
pointer form, whatever `Foo` defines.

Since the address varies from run to run, examples on this site can only check
a fixed prefix of the result, not the full string.

## Example

{{example:types.Class.tostring}}
