---
see_also: [math.floor, math.ceil, math.abs]
---

Rounds `x` to the nearest integer.

## Parameters

- `x` - the number to round

## Return value

The nearest integer value to `x`, as a `float`.

## Notes

A halfway value rounds away from zero, not to the nearest even integer:
`round(2.5)` is `3` and `round(-2.5)` is `-3`.

## Example

{{example:math.round}}
