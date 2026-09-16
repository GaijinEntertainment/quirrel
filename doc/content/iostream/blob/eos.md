---
see_also: [iostream.stream.eos, iostream.stream.tell, iostream.stream.len]
---

Returns non-null exactly when the cursor has reached the end of the blob.

## Return value

See [iostream.stream.eos](sym:iostream.stream.eos) for the generic contract.

## Notes

For a blob this is `tell() == len()`. Both change often on a blob -
`len` grows on a write past the end, `tell` moves on every read, write or
seek - so `eos` reflects the current state of both, not a sticky flag.

## Example

{{example:iostream.blob.eos}}
