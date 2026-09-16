---
see_also: [iostream.stream.readblob, io.file.writeblob, io.file.readn]
---

Reads up to `size` bytes from the cursor and returns them as a new
[blob](sym:iostream.blob).

## Parameters

- `size` - the largest number of bytes to read

## Return value

See [iostream.stream.readblob](sym:iostream.stream.readblob) for the
generic contract.

## Errors

Throws `no data left to read` when the cursor is already at `len` and
nothing can be read. Throws `the stream is invalid` once the file
has been [closed](sym:io.file.close).

## Notes

`size` is capped against `len`, not against what is left after the cursor,
so asking for more than remains is not an error by itself: fewer bytes than
requested come back in the result blob. Only asking with nothing left
throws.

## Example

{{example:io.file.readblob}}
