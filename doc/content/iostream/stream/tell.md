---
see_also: [iostream.blob.tell, iostream.stream.seek, iostream.stream.len]
---

Returns the current cursor position.

## Return value

The cursor's offset from the start of the stream, in bytes.

## Notes

`seek` moves the cursor directly; `readn`, `writen`, `readblob`, `writeblob`,
`readobject` and `writeobject` move it by however many bytes they consume or
produce. The same method works on a `file`.

## Example

{{example:iostream.stream.tell}}
