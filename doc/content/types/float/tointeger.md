---
see_also: [types.Integer.tointeger, types.Float.tofloat, types.Float.tochar, types.Bool.tointeger]
---

Converts the float to an integer.

## Return value

The value truncated toward zero, not rounded: `(3.9).tointeger()` gives `3`
and `(-3.9).tointeger()` gives `-3`, both dropping the fractional part rather
than rounding to the nearest integer.

## Errors

Nothing throws for a value outside the 64-bit integer range, but the result
is then implementation-defined rather than meaningful: converting a float
too large (or too negative) to fit a `SQInteger` is a plain C++
`(SQInteger)` cast on an out-of-range value, which is undefined behavior in
the language standard. On this build's target it happens to saturate to the
same fixed sentinel (`-9223372036854775808`, i.e. `INT64_MIN`) for every
such float, whether the overflow is positive or negative, but that is a
property of the compiler and CPU, not a guarantee - do not rely on the exact
value.

## Notes

Takes no arguments, same as [`types.Integer.tointeger`](sym:types.Integer.tointeger);
no `base` parameter exists here either, unlike
[`types.String.tointeger`](sym:types.String.tointeger).

## Example

{{example:types.Float.tointeger}}
