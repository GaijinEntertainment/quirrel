---
see_also: [iostream.stream.readn, io.file.writen, io.file.seek]
---

Reads a value of the given numeric `format` at the cursor and returns it.

## Parameters

- `format` - a character selecting the type and size to read

## Return value

See [iostream.stream.readn](sym:iostream.stream.readn) for the format
codes, their sizes and the byte order used.

## Errors

Throws `io error` when fewer bytes than the format needs are left before
`len`. Throws `the stream is invalid` once the file has been
[closed](sym:io.file.close).

## Notes

The bytes come straight from the OS file, in whatever mode the file was
opened with; open with a `b` mode (see `constructor`) so no line-ending
translation can land an unexpected byte in the middle of a value.

## Example

{{example:io.file.readn}}
