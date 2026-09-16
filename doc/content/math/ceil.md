---
see_also: [math.floor, math.round, math.abs]
---

Rounds `x` toward positive infinity.

## Parameters

- `x` - the number to round

## Return value

The smallest integer value that is not less than `x`.

## Notes

The result is always a `float`, even when `x` is already an integer. For a
negative `x`, "upward" moves toward zero, not away from it: `ceil(-3.2)` is
`-3`.

## Example

{{example:math.ceil}}
