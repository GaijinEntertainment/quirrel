---
see_also: [math.max, math.clamp]
---

Returns the smallest of `x`, `y` and any further arguments.

## Parameters

- `x` - a number to compare
- `y` - a number to compare
- `...` - more numbers to compare

## Return value

The smallest argument, returned unchanged.

## Errors

Throws `parameter N of 'min' has an invalid type` when an argument is not a
number, and `wrong number of parameters passed to native closure 'min'` when
called with fewer than two arguments.

## Notes

The winner is returned as the original object, so the result keeps its own
type and not `x`'s: `type(min(5, 2.0))` is `float` because `2.0` is the
smaller value, while `type(min(2, 5.0))` is `integer` because `2` is smaller.
When two arguments compare equal, the earlier one in the argument list is
kept.

Every argument is reported by the function's own name and by its true
position, whether it was matched by `x`, by `y`, or by the trailing `...`.

## Example

{{example:math.min}}
