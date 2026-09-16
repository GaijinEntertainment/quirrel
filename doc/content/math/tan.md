---
see_also: [math.sin, math.cos, math.atan, math.PI]
---

Returns the tangent of `x`, given in radians.

## Parameters

- `x` - angle in radians

## Return value

The tangent of `x`, always as a float.

## Notes

Near an odd multiple of `PI/2` the tangent has a vertical asymptote. Quirrel
does not throw there; it returns whatever the underlying C `tan` computes for
the (inexact) argument, typically a very large finite number rather than an
error. That magnitude depends on the float precision the build uses and is
not the same across platforms, so do not print or compare it.

## Example

{{example:math.tan}}
