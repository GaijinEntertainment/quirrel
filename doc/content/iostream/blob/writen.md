---
see_also: [iostream.stream.writen, iostream.blob.readn, iostream.blob.resize]
---

Writes `value` at the cursor, in the given numeric `format`.

## Parameters

- `value` - the value to write
- `format` - a character selecting the type and size to write

## Errors

Throws `invalid format` when `format` is not one of the documented codes.

## Notes

See [iostream.stream.writen](sym:iostream.stream.writen) for the format
codes, their sizes and the byte order used.

Writing past the current `len` grows this blob to fit: unlike a read past
the end, a write past the end is never an error.

## Example

{{example:iostream.blob.writen}}
