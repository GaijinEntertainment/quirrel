---
params: [base]
see_also: [types.String.tofloat]
---

Parses `str` as an integer.

## Parameters

- `base` - the numeric base to parse in, from 2 to 36; defaults to 10
- `...` - not a real parameter; see Notes

## Return value

`str` parsed as an integer in the given `base`.

## Errors

Throws `cannot convert the string to integer` when `str` does not parse as a
number in `base`.

## Notes

Takes 0 or 1 argument, not the 0-or-more the VM's dump implies. This binding
carries no declaration string, so the VM cannot tell an optional parameter
from a variadic tail and dumps it as `tointeger([arg1: number], ...)`. A
second explicit argument is silently ignored, since the underlying C++ only
reads one extra argument.

## Example

{{example:types.String.tointeger}}
