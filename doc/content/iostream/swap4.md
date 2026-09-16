---
see_also: [iostream.swap2, iostream.blob.swap4, iostream.casti2f]
---

Byte-swaps the low 32 bits of `val` and returns them as an integer.

## Parameters

- `val` - the value whose low 32 bits get byte-swapped

## Return value

An `int` in `[0, 4294967295]`: the four low bytes of `val`, in reverse order.

## Notes

Only the low 32 bits of `val` matter; higher bits are discarded, not carried
into the result. Swapping is its own inverse, so `swap4(swap4(val) & 0xFFFFFFFF)`
restores the original low 32 bits.

## Example

{{example:iostream.swap4}}
