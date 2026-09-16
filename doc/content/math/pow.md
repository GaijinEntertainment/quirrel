---
see_also: [math.sqrt, math.exp, math.log]
---

Returns `x` raised to the power `y`.

## Parameters

- `x` - the base
- `y` - the exponent

## Return value

`x` raised to the power `y`, as a float.

## Notes

The result is always a `float`, even when both `x` and `y` are integers:
`type(pow(2, 3))` is `"float"`.

Out-of-domain input does not throw. A negative `x` with a fractional `y`
gives NaN; `x` equal to `0` with a negative `y` gives infinity. Both print
differently between compilers and platforms, so do not print them from
portable code.

## Example

{{example:math.pow}}
