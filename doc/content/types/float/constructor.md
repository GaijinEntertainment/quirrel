---
see_also: [types.Float.tofloat, types.Integer.constructor, types.Bool.constructor]
---

Converts `value` to a float. This is what runs when `types.Float` itself is
called, `types.Float(value)`.

## Return value

`0.0` (printed as `"0"`, see [`tostring`](sym:types.Float.tostring)) when
called with no arguments. Otherwise `value` converted to a float: a float is
returned as is, an integer is widened, a bool gives `0.0` or `1.0`, and a
string is parsed as a decimal number - unlike
[`types.Integer.constructor`](sym:types.Integer.constructor), there is no
`base` argument, since float literals have no base-16 or base-8 form.

## Errors

Throws `cannot convert to Float` for a `table`, `array`, `null` or any other
non-numeric, non-string, non-bool type, and `cannot convert string to Float`
when the string does not parse as a number.

## Notes

Takes zero or one argument, but the VM shows it with the same generic
fallback receiver described on
[`types.String.constructor`](sym:types.String.constructor); extra arguments
beyond the first are accepted and ignored.

## Example

{{example:types.Float.constructor}}
