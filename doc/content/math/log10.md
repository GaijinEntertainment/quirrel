---
see_also: [math.log, math.exp, math.pow]
---

Returns the base-10 logarithm of `x`.

## Parameters

- `x` - the number to take the logarithm of

## Return value

The base-10 logarithm of `x`, as a float.

## Notes

The result is always a `float`, even when `x` is an integer:
`type(log10(100))` is `"float"`.

`log10(0)` does not throw. It returns negative infinity. A negative `x`
does not throw either; it returns NaN. Both print differently between
compilers and platforms, so do not print them from portable code.

## Example

{{example:math.log10}}
