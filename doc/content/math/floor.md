---
see_also: [math.ceil, math.round, math.abs]
---

Rounds `x` toward negative infinity.

## Parameters

- `x` - the number to round

## Return value

The largest integer value that is not greater than `x`.

## Notes

The result is always a `float`, even when `x` is already an integer:
`type(floor(3))` gives `"float"`, not `"integer"`. Convert with `int(...)` when
an integer value is required.

## Example

{{example:math.floor}}
