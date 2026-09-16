---
see_also: [iostream.blob.eos, iostream.stream.tell, iostream.stream.len]
---

Reports whether the cursor is at the end of the stream.

## Return value

`1` when `tell() == len()`, `null` otherwise. It is computed fresh from both
on every call, not a sticky flag that a later `seek` or write leaves behind.

## Notes

The same method works on a `file`. A read or write that consumes exactly the
last remaining byte lands the cursor on `len()`, so `eos` turns non-null right
after that call, before any further read is attempted.

## Example

{{example:iostream.stream.eos}}
