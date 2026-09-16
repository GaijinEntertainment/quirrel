---
see_also: [iostream.blob.resize, iostream.stream.len, iostream.stream.tell]
---

Creates a blob of `size` bytes, all zero, with the cursor at 0.

## Parameters

- `size` - number of bytes to allocate

## Errors

Throws `cannot create blob with negative size` when `size` is negative.

## Notes

`size` defaults to 0, which makes an empty blob rather than failing; writing
to it later grows it, as described on `writen`.

## Example

{{example:iostream.blob.constructor}}
