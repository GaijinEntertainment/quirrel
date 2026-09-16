---
see_also: [iostream.stream.writeblob, iostream.blob.readblob, iostream.blob.writestring]
---

Writes all of `blob` at the cursor and returns the number of bytes written.

## Parameters

- `blob` - the source blob; all of it is written

## Return value

See [iostream.stream.writeblob](sym:iostream.stream.writeblob) for the
generic contract.

## Notes

Writing past the current `len` grows this blob to fit, the same as `writen`;
it is never an error to write more than currently fits.

## Example

{{example:iostream.blob.writeblob}}
