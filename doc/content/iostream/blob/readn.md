---
see_also: [iostream.stream.readn, iostream.blob.writen, iostream.blob.seek]
---

Reads a value of the given numeric `format` at the cursor and returns it.

## Parameters

- `format` - a character selecting the type and size to read

## Return value

See [iostream.stream.readn](sym:iostream.stream.readn) for the format
codes, their sizes and the byte order used.

## Errors

Throws `io error` when fewer bytes than the format needs are left before
`len`.

## Notes

This is not the same as indexing with `blob[i]`: indexing reads a single raw
byte at an absolute position, does not move the cursor, and applies no
numeric conversion, while `readn` converts a fixed number of bytes starting
at the cursor and advances it by that many bytes.

## Example

{{example:iostream.blob.readn}}
