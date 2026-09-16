---
see_also: [math.log, math.pow]
---

Returns e raised to the power of `x`.

## Parameters

- `x` - the exponent

## Return value

e raised to the power of `x`, as a float.

## Notes

The result is always a `float`, even when `x` is an integer:
`type(exp(0))` is `"float"`.

A large enough `x` does not throw. It returns infinity, an ordinary float,
not an error. A large enough negative `x` returns `0.0` rather than
underflowing to an error.

## Example

{{example:math.exp}}
