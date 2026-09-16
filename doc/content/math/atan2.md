---
see_also: [math.atan, math.sin, math.cos, math.PI]
---

Returns the arctangent of `y/x`, using the sign of each argument to pick the
correct quadrant.

## Parameters

- `y` - numerator; the sign tells which half-plane the angle is in
- `x` - denominator; the sign tells which half-plane the angle is in

## Return value

The angle in radians of the point `(x, y)`, in the range `[-PI, PI]`, always
as a float.

## Notes

Because the two arguments carry separate signs, `atan2` places the angle in
the correct quadrant, which `atan(y/x)` alone cannot do. `atan2(0, 0)` is
left undefined by plain mathematics and by some C libraries, but Quirrel's
build defines it as `0` rather than `nan`.

## Example

{{example:math.atan2}}
