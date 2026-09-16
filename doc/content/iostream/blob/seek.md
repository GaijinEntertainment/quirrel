---
see_also: [iostream.stream.seek, iostream.stream.tell, iostream.blob.resize]
---

Moves the cursor to `offset`, relative to `origin`.

## Parameters

- `offset` - how far to move
- `origin` - `'b'` (begin), `'c'` (current) or `'e'` (end); defaults to `'b'`

## Return value

See [iostream.stream.seek](sym:iostream.stream.seek) for the generic
contract.

## Errors

Throws `invalid origin` when `origin` is not `'b'`, `'c'` or `'e'`.

## Notes

`'e'` and the bounds of the seek are relative to `len`, not to any
allocated capacity. Unlike `readn` and `writen`, a seek outside the blob does
not throw: it just returns -1 and leaves the cursor where it was.

## Example

{{example:iostream.blob.seek}}
