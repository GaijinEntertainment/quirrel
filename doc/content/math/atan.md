---
see_also: [math.atan2, math.asin, math.acos, math.PI]
---

Returns the arctangent of `x`, in radians.

## Parameters

- `x` - value whose arctangent is wanted

## Return value

The arctangent of `x`, in the range `[-PI/2, PI/2]`, always as a float.

## Notes

Unlike [asin](sym:math.asin) and [acos](sym:math.acos), `x` is defined for
every real number, so there is no domain to clamp.

## Example

{{example:math.atan}}
