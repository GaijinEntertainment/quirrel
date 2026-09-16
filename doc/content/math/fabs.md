---
see_also: [math.abs, math.floor, math.ceil]
---

Returns the absolute value of `x`, converted to `float`.

## Parameters

- `x` - the number to take the absolute value of

## Return value

The absolute value of `x`, always as a `float`, even when `x` is an integer.

## Notes

Unlike `abs`, the result type never depends on the argument's type. Use
`abs` when the result should keep the argument's own type.

## Example

{{example:math.fabs}}
