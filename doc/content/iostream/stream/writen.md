---
see_also: [iostream.blob.writen, iostream.stream.readn, iostream.stream.seek, iostream.swap2]
---

Writes `value` at the cursor, in the given numeric `format`, and advances the
cursor by the format's width.

## Parameters

- `value` - the value to write
- `format` - a character selecting the numeric type and width to write

## Notes

`readn` reads back the same eight codes at the same widths:

- `'b'` - `unsigned char`, 1 byte
- `'c'` - `signed char`, 1 byte
- `'w'` - `unsigned short`, 2 bytes
- `'s'` - `short`, 2 bytes
- `'i'` - a 32-bit signed integer, 4 bytes
- `'l'` - the VM's native integer, 8 bytes (this build always uses a 64-bit
  integer, even when the target is 32-bit)
- `'f'` - a 32-bit IEEE float, 4 bytes
- `'d'` - a 64-bit IEEE double, 8 bytes; the value stored is only ever as
  precise as the 32-bit `float` this build's `number` already narrowed it to

Bytes are stored verbatim, in the host's native byte order (little-endian on
every platform this engine targets); `writen` never swaps bytes on its own.
Pair it with a byte swap, such as [iostream.swap2](sym:iostream.swap2) or
[iostream.swap4](sym:iostream.swap4), before handing the bytes to a
big-endian peer.

The same method works on a `file`: it writes at the file's current position
and advances it by the format's width, the same as it moves a blob's cursor.
Writing past the current `len` grows a blob to fit; writing past the end of a
file extends the file the same way, since both are ordinary writes to the
underlying medium.

## Errors

Throws `invalid format` when `format` is not one of the eight codes above.

`writen` also throws `io error` if the underlying stream cannot store the
full width, for example a `file` opened read-only; a blob's own storage never
refuses a write, so this case cannot happen on a blob.

## Example

{{example:iostream.stream.writen}}
