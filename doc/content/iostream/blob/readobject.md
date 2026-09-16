---
see_also: [iostream.stream.readobject, iostream.blob.writeobject]
---

Deserializes an object from the cursor.

## Parameters

- `classes` - classes the serialized data is allowed to instantiate

## Return value

See [iostream.stream.readobject](sym:iostream.stream.readobject) for the
generic contract.

## Notes

Pairing `writeobject` and `readobject` on the same in-memory blob round-trips
a value without touching a file.

## Example

{{example:iostream.blob.readobject}}
