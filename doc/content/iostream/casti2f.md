---
see_also: [iostream.castf2i, iostream.swap4]
---

Reinterprets the bits of `i` as a float, with no numeric conversion.

## Parameters

- `i` - the value whose low 32 bits become the result

## Return value

The `float` whose bit pattern equals the low 32 bits of `i`.

## Notes

A float is 4 bytes, so only the low 32 bits of `i` matter; any bits above
that are ignored, even though `i` itself can hold a wider integer.

## Example

{{example:iostream.casti2f}}
