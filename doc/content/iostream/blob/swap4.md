---
see_also: [iostream.blob.swap2, iostream.swap4]
---

Byte-swaps the whole blob in place, taken as an array of 32-bit values.

## Return value

None; the blob is modified in place.

## Notes

Works in whole 4-byte units starting at byte 0, over the entire buffer; any
trailing 1-3 bytes left over from `len % 4` are untouched. The cursor (see
`tell`) does not move.

Swapping is its own inverse, so calling `swap4` twice restores the original
bytes.

## Example

{{example:iostream.blob.swap4}}
