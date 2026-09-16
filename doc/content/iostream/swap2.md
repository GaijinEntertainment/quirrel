---
see_also: [iostream.swap4, iostream.blob.swap2]
---

Byte-swaps the low 16 bits of `val` and returns them as an integer.

## Parameters

- `val` - the value whose low 16 bits get byte-swapped

## Return value

An `int` in `[0, 65535]`: the two low bytes of `val`, in reverse order.

## Notes

Only the low 16 bits of `val` matter; higher bits are discarded, not carried
into the result. Swapping is its own inverse, so `swap2(swap2(val) & 0xFFFF)`
restores the original low 16 bits.

## Example

{{example:iostream.swap2}}
