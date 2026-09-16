---
see_also: [types.String.tointeger]
---

Parses `str` as a float.

## Return value

`str` parsed as a float, using the same syntax the compiler accepts for a
float literal.

## Errors

Throws `cannot convert the string to float` when `str` does not parse as a
number.

## Notes

Takes no arguments beyond the string itself; unlike `types.String.tointeger`,
there is no base to pass, and the check is exact (calling it with an extra
argument throws a wrong-number-of-parameters error rather than ignoring it).

## Example

{{example:types.String.tofloat}}
