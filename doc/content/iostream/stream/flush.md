---
see_also: [iostream.blob.flush, iostream.stream.writen, iostream.stream.writeblob]
---

Flushes any buffered writes and reports whether it succeeded.

## Return value

`1` on success, `null` on failure.

## Notes

A blob has nothing buffered, so `flush` on a blob always succeeds. The same
method works on a `file`, where it flushes the C library's write buffer for
that file; it can return `null` there if the underlying write fails.

## Example

{{example:iostream.stream.flush}}
