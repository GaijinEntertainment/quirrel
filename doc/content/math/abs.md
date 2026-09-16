---
see_also: [math.fabs, math.floor, math.ceil]
---

Returns the absolute value of `x`.

## Parameters

- `x` - the number to take the absolute value of

## Return value

`x` with its sign removed. The result keeps the type of `x`: an integer
argument gives an integer back, a float argument gives a float back. Use
`fabs` when a float result is wanted regardless of the argument's type.

## Notes

The smallest representable integer has no positive counterpart at the same
width, so taking its absolute value overflows and returns that same negative
value unchanged.

## Example

{{example:math.abs}}
