---
see_also: [iostream.stream.writestring, io.file.writeblob, io.file.constructor]
---

Writes `str` at the cursor and returns the number of characters written.

## Parameters

- `str` - the string to write

## Return value

See [iostream.stream.writestring](sym:iostream.stream.writestring) for
the generic contract.

## Errors

Throws `the stream is invalid` once the file has been [closed](sym:io.file.close).

## Notes

The count returned is the raw byte length of `str`; it is exact only in a
`b` mode. In a text mode (see `constructor`), the C library can insert an
extra byte per line ending, so more bytes can land on disk than the
returned count says were written.

## Example

{{example:io.file.writestring}}
