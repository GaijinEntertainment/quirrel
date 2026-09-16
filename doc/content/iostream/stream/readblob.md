---
see_also: [iostream.blob.readblob, iostream.stream.writeblob, iostream.stream.readn]
---

Reads up to `size` bytes at the cursor into a new blob, advances the cursor
by the number of bytes read, and returns that blob.

## Parameters

- `size` - the largest number of bytes to read

## Return value

A new blob holding the bytes read. `size` is capped against `len()` first, so
asking for more than the stream holds is not an error by itself; the result
can be smaller still, capped again by how many bytes remain after the
cursor. Only when that second cap leaves nothing, including when `size`
itself was `0`, does `readblob` throw instead of returning an empty blob.

## Errors

Throws `invalid size` when `size` is negative.

Throws `no data left to read` when zero bytes could be read: the cursor is
already at `len()`, or `size` capped down to `0`.

## Notes

The same method works on a `file`: it reads at the file's current position,
capped the same way against the file's size.

## Example

{{example:iostream.stream.readblob}}
