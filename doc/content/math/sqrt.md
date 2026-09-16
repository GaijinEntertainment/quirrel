---
see_also: [math.pow, math.exp]
---

Returns the square root of `x`.

## Parameters

- `x` - the number to take the square root of

## Return value

The square root of `x`, as a float.

## Notes

The result is always a `float`, even when `x` is an integer:
`type(sqrt(4))` is `"float"`.

A negative `x` does not throw. It returns NaN, and NaN prints differently
between compilers and platforms, so do not print it from portable code.

## Example

{{example:math.sqrt}}
