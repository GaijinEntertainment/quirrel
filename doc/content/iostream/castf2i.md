---
see_also: [iostream.casti2f, iostream.swapfloat, iostream.swap4]
---

Reinterprets the bits of `f` as an integer, with no numeric conversion.

## Parameters

- `f` - the value whose bits become the result

## Return value

The 32-bit pattern of `f`, as a non-negative `int`: the float's sign bit
becomes bit 31 of the result rather than making it negative.

## Notes

`f` is read as a `float` first, so an integer argument is converted to the
float of that value before its bits are taken; `castf2i(1)` and `castf2i(1.0)`
give the same result, which is the bit pattern of `1.0`, not of the integer 1.

Only the low 32 bits of the result are meaningful; the upper bits are always 0.

## Example

{{example:iostream.castf2i}}
