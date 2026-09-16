---
see_also: [iostream.stream.readobject, io.file.writeobject, io.file.constructor]
---

Deserializes an object from the cursor.

## Parameters

- `classes` - classes the serialized data is allowed to instantiate

## Return value

See [iostream.stream.readobject](sym:iostream.stream.readobject) for the
generic contract.

## Errors

Throws `the stream is invalid` once the file has been
[closed](sym:io.file.close). See
[iostream.stream.readobject](sym:iostream.stream.readobject) for the
deserialization errors, such as reading data that `writeobject` did not
produce.

## Notes

Pairing `writeobject` and `readobject` on a file persists a value across
runs, not just within one script: write it, close that handle, and a later
run opens the same path to read it back. Reading it back with the same
still-open handle also works, provided the cursor is moved back first (see
`seek`) - the same rule as reading anything else just written.

## Example

{{example:io.file.readobject}}
