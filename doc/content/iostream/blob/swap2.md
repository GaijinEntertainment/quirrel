---
see_also: [iostream.blob.swap4, iostream.swap2]
---

Byte-swaps the whole blob in place, taken as an array of 16-bit values.

## Return value

None; the blob is modified in place.

## Notes

Works in whole 2-byte units starting at byte 0, over the entire buffer; if
`len` is odd, the last byte is left untouched. The cursor (see `tell`) does
not move.

Swapping is its own inverse, so calling `swap2` twice restores the original
bytes.

## Example

{{example:iostream.blob.swap2}}
