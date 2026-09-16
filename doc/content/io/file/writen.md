---
see_also: [iostream.stream.writen, io.file.readn, io.file.constructor]
---

Writes `value` at the cursor, in the given numeric `format`.

## Parameters

- `value` - the value to write
- `format` - a character selecting the type and size to write

## Errors

Throws `the stream is invalid` once the file has been [closed](sym:io.file.close).

## Notes

See [iostream.stream.writen](sym:iostream.stream.writen) for the format
codes, their sizes and the byte order used. Open with a `b` mode (see
`constructor`) so no line-ending translation can land an unexpected byte
in the middle of a value.

## Example

{{example:io.file.writen}}
