---
see_also: [iostream.stream.writeobject, io.file.readobject, io.file.constructor]
---

Serializes `obj` to the cursor.

## Parameters

- `obj` - the value to serialize
- `classes` - classes the serialized data is allowed to instantiate

## Errors

Throws `the stream is invalid` once the file has been
[closed](sym:io.file.close). See
[iostream.stream.writeobject](sym:iostream.stream.writeobject) for the
serialization errors.

## Notes

See [iostream.stream.writeobject](sym:iostream.stream.writeobject) for the
generic contract. `close` the file (or `flush` it) before another process
or another `file` instance tries to read the bytes back with
`readobject`.

## Example

{{example:io.file.writeobject}}
