---
see_also: [types.Integer.tointeger, types.Integer.tochar, types.Float.tofloat, types.Bool.tofloat]
---

Converts the integer to a float.

## Return value

The nearest `float` to the integer's value.

## Errors

Nothing throws here, but the conversion is lossy: `SQFloat` is a 32-bit
`float` in this build, while `SQInteger` is 64-bit, so an integer needs no
more than about 7 significant decimal digits before `tofloat()` cannot
represent it exactly any more. `9007199254740993` (2^53 + 1) round-trips
through a `double` in most languages but not through this 32-bit `float`:
`tofloat()` rounds it to the nearest representable value, and converting that
back with [`tointeger`](sym:types.Float.tointeger) does not reproduce the
original integer.

## Notes

Takes no arguments. See [`types.Float.tofloat`](sym:types.Float.tofloat),
which is the identity conversion and does not round.

## Example

{{example:types.Integer.tofloat}}
