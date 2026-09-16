---
see_also: [iostream.stream.writeblob, io.file.readblob, io.file.writestring]
---

Writes all of `blob` at the cursor and returns the number of bytes written.

## Parameters

- `blob` - the source blob; all of it is written

## Return value

See [iostream.stream.writeblob](sym:iostream.stream.writeblob) for the
generic contract.

## Errors

Throws `the stream is invalid` once the file has been [closed](sym:io.file.close).

## Notes

Writing past the current end of the file grows it, the same as `writen`;
it is never an error to write more than currently fits, since a real file
has no fixed capacity to run into.

## Example

{{example:io.file.writeblob}}
