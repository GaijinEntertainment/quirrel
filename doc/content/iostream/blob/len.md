---
see_also: [iostream.stream.len, iostream.stream.tell, iostream.blob.resize]
---

Returns the blob's current length.

## Return value

See [iostream.stream.len](sym:iostream.stream.len) for the generic
contract.

## Notes

This is the length set by the constructor or `resize`, or grown implicitly
by a write past the end (see `writen`). It does not depend on the cursor;
use `tell` for that.

## Example

{{example:iostream.blob.len}}
