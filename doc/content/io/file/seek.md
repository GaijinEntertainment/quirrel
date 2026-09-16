---
see_also: [iostream.stream.seek, io.file.tell, io.file.len]
---

Moves the cursor to `offset`, relative to `origin`.

## Parameters

- `offset` - how far to move
- `origin` - `'b'` (begin), `'c'` (current) or `'e'` (end); defaults to `'b'`

## Return value

See [iostream.stream.seek](sym:iostream.stream.seek) for the generic
contract.

## Errors

Throws `invalid origin` when `origin` is not `'b'`, `'c'` or `'e'`. Throws
`the stream is invalid` once the file has been [closed](sym:io.file.close).

## Notes

Unlike a [blob](sym:iostream.blob.seek), seeking past the real end of a
file is not an error: the OS allows it, `seek` returns success, and `tell`
reports the new position, even though nothing was written there. A read
at that position still fails, and a write there grows the file, leaving a
hole that reads back as zero bytes.

A seek that resolves before byte 0 fails and returns -1; whether the
cursor itself moves on that failure can depend on the platform's C library
and on whether the last operation was a read, so treat -1 as the only
guarantee and re-seek explicitly afterward if the position matters.

## Example

{{example:io.file.seek}}
