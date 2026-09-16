---
params: [value]
see_also: [string.format, types.String.replace]
---

Substitutes `{0}`, `{1}`, ... and `{name}` placeholders in `str` with the
given values.

## Parameters

- `value` - the first substitution value
- `...` - further substitution values

## Return value

`str` with each `{N}` replaced by `tostring()` of the N-th value (0-based,
counting from `value`), and each `{name}` replaced by `tostring()` of
`name`'s entry in the first table among the values that has that key. A
placeholder with no matching value or key is left in the result as
written, including its braces.

## Errors

Throws `subst: Failed to convert value to string` if a substituted value's
`tostring()` fails. Every built-in type converts successfully.

## Notes

Takes 1 or more arguments: at least one value is required, even if `str` has
no placeholders - calling it with zero explicit arguments throws a
wrong-number-of-parameters error. This is a real, unbounded vararg, unlike
the fake `...` the VM's dump adds to several other methods on this page for
unrelated reasons.

Differs from [string.format](sym:string.format): `subst` looks up values by
position or by table key inside `{}`, with no `%`-style conversion syntax and
no fixed argument count, while `format` walks a C `printf`-style pattern and
requires exactly as many arguments as it has conversions.

## Example

{{example:types.String.subst}}
