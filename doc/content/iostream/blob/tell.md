---
see_also: [iostream.stream.tell, iostream.stream.seek, iostream.blob.len]
---

Returns the current cursor position.

## Return value

See [iostream.stream.tell](sym:iostream.stream.tell) for the generic
contract.

## Notes

This is the same cursor that `seek`, `readn`, `writen`, `readblob` and
`writeblob` move; indexing a blob with `[]` does not move it.

## Example

{{example:iostream.blob.tell}}
