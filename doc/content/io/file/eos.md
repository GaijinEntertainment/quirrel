---
see_also: [iostream.stream.eos, io.file.tell, io.file.len, io.file.close]
---

Returns non-null exactly when the cursor has reached the real end of the
file.

## Return value

See [iostream.stream.eos](sym:iostream.stream.eos) for the generic
contract.

## Errors

Throws `the stream is invalid` once the file has been [closed](sym:io.file.close).

## Notes

This is `tell() == len()`, so it costs the same two extra seeks as `len`
(see its Notes) every time it is called. Seeking past the real end moves
`tell` without moving `len`, so `eos` correctly reports false there, even
though a read at that position fails.

## Example

{{example:io.file.eos}}
