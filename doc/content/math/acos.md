---
see_also: [math.asin, math.atan, math.cos, math.PI]
---

Returns the arccosine of `x`, in radians.

## Parameters

- `x` - value whose arccosine is wanted

## Return value

The arccosine of `x`, in the range `[0, PI]`, always as a float.

## Notes

The mathematical arccosine is only defined for `x` in `[-1, 1]`; C's `acos`
answers a domain error with `nan` outside it. Quirrel avoids that: `x` is
clamped to `[-1, 1]` before the call, so `acos(2)` quietly returns the same
value as `acos(1)` instead of `nan`.

## Example

{{example:math.acos}}
