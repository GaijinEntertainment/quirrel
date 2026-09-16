---
see_also: [math.min, math.max, math.abs]
---

Clamps `x` to the closed range `[min, max]`.

## Parameters

- `x` - value to clamp
- `min` - lower bound of the range
- `max` - upper bound of the range

## Return value

`min` if `x` is below the range, `max` if it is above, otherwise `x` itself.

## Errors

Throws `Invalid clamp range: min>max` when `min > max`. The range is checked
before `x` is read, so an inverted range always throws, whatever `x` is.

## Notes

The bound that wins is returned unchanged, so the result keeps the type of that
argument and not of `x`: `clamp(15, 0, 10.0)` gives the float `10.0`, while
`clamp(5, 0.0, 10.0)` gives the integer `5`. Keep the bounds and the value in one
type when the caller expects a fixed result type.

Comparison is raw, so a `_cmp` metamethod on an instance is not consulted.

## Example

{{example:math.clamp}}
