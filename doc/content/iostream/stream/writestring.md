---
see_also: [iostream.blob.writestring, iostream.stream.writeblob, iostream.stream.readblob]
---

Writes `str` at the cursor and advances the cursor by its length.

## Parameters

- `str` - the string to write

## Return value

The number of bytes written: `str`'s byte length, the same number `str.len()`
reports. A string holding multi-byte UTF-8 text counts its encoded bytes, not
its number of code points.

## Errors

Throws `io error` if the underlying stream cannot store the full amount, for
example a `file` opened read-only; a blob's own storage never refuses a
write, so this case cannot happen on a blob.

## Notes

The same method works on a `file`. Writing past the current `len` grows a
blob to fit, the same as `writen`; writing past the end of a writable file
extends it the same way.

## Example

{{example:iostream.stream.writestring}}
