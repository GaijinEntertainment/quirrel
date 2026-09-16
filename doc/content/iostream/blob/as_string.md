---
see_also: [iostream.stream.readblob, iostream.blob.tostring]
---

Returns the whole blob, from byte 0 to `len`, as a string.

## Return value

A string of `len` bytes. It can contain embedded zero bytes, since the
length is passed along explicitly rather than found by scanning for a
terminator.

## Notes

Unlike `readblob`, `as_string` always starts at byte 0 and ignores the
cursor: it does not consult or move `tell`. Read the blob through `readblob`
or `readn` instead when the cursor position should matter.

## Example

{{example:iostream.blob.as_string}}
