---
see_also: [math.log10, math.exp, math.pow]
---

Returns the natural logarithm of `x`.

## Parameters

- `x` - the number to take the logarithm of

## Return value

The natural logarithm of `x`, as a float.

## Notes

The result is always a `float`, even when `x` is an integer:
`type(log(1))` is `"float"`.

`log(0)` does not throw. It returns negative infinity. A negative `x`
does not throw either; it returns NaN. Both print differently between
compilers and platforms, so do not print them from portable code.

## Example

{{example:math.log}}
