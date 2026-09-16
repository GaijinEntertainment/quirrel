---
see_also: [iostream.blob.writeobject, iostream.stream.readobject, iostream.stream.writeblob]
---

Serializes `obj` into a self-contained block of bytes at the cursor, and
advances the cursor past it.

## Parameters

- `obj` - the value to serialize
- `classes` - a table from class name to class, needed only when `obj`
  contains an instance

## Errors

Throws `Unsupported object type for serialization` for a value `writeobject`
cannot encode: a function, generator, thread, weakref, userdata or
userpointer, whether `obj` is that value itself or it turns up nested inside
an array or table.

An instance of a class needs `classes` to name that class, and the class
needs a `__getstate` method that returns the state to save:

- `Unsupported class for serialization` - the instance's class is not a key
  in `classes` (including when `classes` was not passed)
- `Instance must have __getstate method for serialization` - the class has
  no `__getstate`
- `Instance method __getstate must be a closure` - the class has a
  `__getstate` slot, but it is not callable

## Notes

The same method works on a `file`. Writing past the current `len` grows a
blob to fit, the same as `writen`.

[iostream.stream.readobject](sym:iostream.stream.readobject) reverses this:
pairing the two on the same stream round-trips a value, provided `classes`
lists every class either side needs.

## Example

{{example:iostream.stream.writeobject}}
