---
see_also: [iostream.stream.writeobject, iostream.blob.readobject]
---

Serializes `obj` to the cursor.

## Parameters

- `obj` - the value to serialize
- `classes` - classes the serialized data is allowed to instantiate

## Notes

See [iostream.stream.writeobject](sym:iostream.stream.writeobject) for the
generic contract. Writing past the current `len` grows this blob to fit, the
same as `writen`.

## Example

{{example:iostream.blob.writeobject}}
