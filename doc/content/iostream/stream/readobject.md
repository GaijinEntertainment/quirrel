---
see_also: [iostream.blob.readobject, iostream.stream.writeobject, iostream.stream.readblob]
---

Deserializes a value from the block of bytes at the cursor, advances the
cursor past it, and returns the value.

## Parameters

- `classes` - a table from class name to class, needed only when the data
  contains an instance

## Return value

The value `writeobject` encoded: `null`, a `bool`, an `int`, a `float`, a
string, an array, a `table`, or an instance of a class named in `classes`.
Building an instance calls that class's constructor and then its
`__setstate` method with the saved state.

## Errors

Throws `Unexpected end of data during deserialization` when the stream runs
out of bytes before a complete value has been read, including when nothing
is left, and `Invalid start marker during deserialization` or
`Invalid end marker during deserialization` when the bytes at the cursor are
not what `writeobject` produces, for example a stream not created by it, or
one read from the wrong position.

An instance in the data needs `classes` to resolve it, and that class needs a
`__setstate` method to receive the saved state:

- `Instance found during deserialization, but available classes not set or
  not a table` - `classes` is missing or not a table
- `Class not found in available classes during deserialization` - `classes`
  does not have the serialized class's name as a key
- `Class must have __setstate method for deserialization` - the class has no
  `__setstate`

## Notes

The same method works on a `file`. Pairing `writeobject` and `readobject` on
the same stream round-trips a value, provided `classes` lists every class
either side needs; see
[iostream.stream.writeobject](sym:iostream.stream.writeobject) for what it
can encode.

## Example

{{example:iostream.stream.readobject}}
