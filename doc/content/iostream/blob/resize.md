---
see_also: [iostream.blob.constructor, iostream.stream.len, iostream.stream.seek]
---

Sets the blob's length to `size`, reallocating its buffer.

## Parameters

- `size` - the new length, in bytes

## Errors

Throws `resize failed, cur size=<n>, requested=<size>` when `size` is
negative.

## Notes

Every call reallocates the buffer, even to shrink it. Shrinking and then
growing back does not recover the bytes that were cut off: the regrown tail
comes back zero-filled, not with its old contents.

If the cursor (see `tell`) is beyond the new, smaller length, it is pulled
back to `size`.

## Example

{{example:iostream.blob.resize}}
