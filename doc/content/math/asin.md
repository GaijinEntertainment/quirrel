---
see_also: [math.acos, math.atan, math.sin, math.PI]
---

Returns the arcsine of `x`, in radians.

## Parameters

- `x` - value whose arcsine is wanted

## Return value

The arcsine of `x`, in the range `[-PI/2, PI/2]`, always as a float.

## Notes

The mathematical arcsine is only defined for `x` in `[-1, 1]`; C's `asin`
answers a domain error with `nan` outside it. Quirrel avoids that: `x` is
clamped to `[-1, 1]` before the call, so `asin(2)` quietly returns the same
value as `asin(1)` instead of `nan`.

## Example

{{example:math.asin}}
