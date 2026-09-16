---
see_also: [math.min, math.clamp]
---

Returns the largest of `x`, `y` and any further arguments.

## Parameters

- `x` - a number to compare
- `y` - a number to compare
- `...` - more numbers to compare

## Return value

The largest argument, returned unchanged.

## Errors

Throws `parameter N of 'max' has an invalid type` when an argument is not a
number, and `wrong number of parameters passed to native closure 'max'` when
called with fewer than two arguments.

## Notes

The winner is returned as the original object, so the result keeps its own
type and not `x`'s: `type(max(5, 8.0))` is `float` because `8.0` is the
larger value, while `type(max(8, 5.0))` is `integer` because `8` is larger.
When two arguments compare equal, the earlier one in the argument list is
kept.

Every argument is reported by the function's own name and by its true
position, whether it was matched by `x`, by `y`, or by the trailing `...`.

## Example

{{example:math.max}}
