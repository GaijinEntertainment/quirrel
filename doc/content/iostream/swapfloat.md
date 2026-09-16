---
see_also: [iostream.swap4, iostream.castf2i]
---

Byte-swaps the 4 bytes of `val`, read as a float, and returns the result.

## Parameters

- `val` - the value, read as a float, whose bytes get swapped

## Return value

The float made of the 4 bytes of `val` in reverse order.

## Notes

`val` is read as a `float` first, so an integer argument is converted to the
float of that value before swapping, the same as in `castf2i`.

Reversing the bytes of an ordinary-looking float rarely gives another
ordinary-looking float: the result is typically a subnormal value with a
tiny magnitude, or on some builds a flush to zero. Do not expect
`swapfloat(x)` to look like `x`; only the round trip is predictable, since
swapping is its own inverse.

## Example

{{example:iostream.swapfloat}}
