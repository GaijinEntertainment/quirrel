---
see_also: [iostream.stream.flush, io.file.close, io.file.constructor]
---

Flushes the file's OS buffer to disk and returns non-null on success.

## Return value

See [iostream.stream.flush](sym:iostream.stream.flush) for the generic
contract. Unlike a blob, a file has a real buffer to flush, so this does
real work and, in principle, can fail.

## Errors

Throws `the stream is invalid` once the file has been [closed](sym:io.file.close).

## Notes

`close` flushes too, so an explicit `flush` before closing is only useful
to make data visible to another reader of the same file while this handle
stays open.

## Example

{{example:io.file.flush}}
