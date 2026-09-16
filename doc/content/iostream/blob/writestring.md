---
see_also: [iostream.stream.writestring, iostream.blob.writeblob, iostream.blob.as_string]
---

Writes `str` at the cursor and returns the number of characters written.

## Parameters

- `str` - the string to write

## Return value

See [iostream.stream.writestring](sym:iostream.stream.writestring) for
the generic contract.

## Notes

Writing past the current `len` grows this blob to fit, the same as `writen`.

## Example

{{example:iostream.blob.writestring}}
