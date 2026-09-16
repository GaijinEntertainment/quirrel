---
see_also: [iostream.stream.len, io.file.tell, io.file.eos, io.file.seek]
---

Returns the file's current size in bytes.

## Return value

See [iostream.stream.len](sym:iostream.stream.len) for the generic
contract.

## Errors

Throws `the stream is invalid` once the file has been [closed](sym:io.file.close).

## Notes

A file has no stored length field: every call seeks to the real end,
reads the position, and seeks back to where the cursor was, so `len` costs
two extra seeks. Prefer keeping a running total in script code over
calling `len` in a loop.

`len` does not move, and is not moved by, the cursor: seeking past the
real end (see `seek`) changes `tell` without changing `len`, since nothing
has been written out there yet.

## Example

{{example:io.file.len}}
