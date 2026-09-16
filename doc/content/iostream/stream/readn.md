---
see_also: [iostream.blob.readn, iostream.stream.writen, iostream.stream.seek, iostream.swap2]
---

Reads a value of the given numeric `format` at the cursor, advances the cursor
by the format's width, and returns the value.

## Parameters

- `format` - a character selecting the numeric type and width to read

## Return value

An `int` for every integer format, a `float` for `'f'` and `'d'`.

- `'b'` - `unsigned char`, 1 byte
- `'c'` - `signed char`, 1 byte
- `'w'` - `unsigned short`, 2 bytes
- `'s'` - `short`, 2 bytes
- `'i'` - a 32-bit signed integer, 4 bytes
- `'l'` - the VM's native integer, 8 bytes (this build always uses a 64-bit
  integer, even when the target is 32-bit)
- `'f'` - a 32-bit IEEE float, 4 bytes
- `'d'` - a 64-bit IEEE double, 8 bytes, narrowed to the 32-bit `float` this
  build's `number` uses

`writen` writes the same eight codes at the same widths.

Bytes are copied verbatim, in the host's native byte order (little-endian on
every platform this engine targets); `readn` never swaps bytes on its own. A
value written and read back on the same platform always round-trips, but
exchanging the raw bytes with a big-endian peer needs a byte swap first, such
as [iostream.swap2](sym:iostream.swap2) or [iostream.swap4](sym:iostream.swap4).

## Errors

Throws `io error` when fewer bytes than the format's width remain before
`len`.

Throws `invalid format` when `format` is not one of the eight codes above.

## Notes

The same method works on a `file`: it reads at the file's current position and
advances it by the format's width, the same as it moves a blob's cursor.

## Example

{{example:iostream.stream.readn}}
