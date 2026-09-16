---
see_also: [iostream.stream.tell, io.file.seek, io.file.len]
---

Returns the current cursor position.

## Return value

See [iostream.stream.tell](sym:iostream.stream.tell) for the generic
contract.

## Errors

Throws `the stream is invalid` once the file has been [closed](sym:io.file.close).

## Notes

What a fresh handle in mode `a` reports before the first write is up to the C
runtime: 0 on Windows, the end of the file on Linux. The write lands at the
real end either way, and after it `tell` reports the true position.

This is the same cursor that `seek`, `readn`, `writen`, `readblob` and
`writeblob` move.

## Example

{{example:io.file.tell}}
