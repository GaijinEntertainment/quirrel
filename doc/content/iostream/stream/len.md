---
see_also: [iostream.blob.len, iostream.stream.tell, iostream.stream.writen]
---

Returns the stream's current length, in bytes.

## Return value

The number of bytes the stream currently holds. This does not depend on the
cursor; use `tell` for that.

## Notes

The same method works on a `file`, where it reports the file's size: writing
past the current length grows the stream to fit, on a blob and on a writable
file alike.

## Example

{{example:iostream.stream.len}}
