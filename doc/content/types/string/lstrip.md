---
see_also: [types.String.strip, types.String.rstrip, string.lstrip]
---

Returns `str` with white space removed from the beginning only.

## Return value

A new string with the leading white space removed; trailing white space, if
any, is kept. See [string.lstrip](sym:string.lstrip) for which bytes
count as white space.

## Notes

Takes no arguments. Same implementation as [string.lstrip](sym:string.lstrip);
see the Notes on [types.String.strip](sym:types.String.strip) for how the two
forms differ only in their reported attributes, not their behavior.

## Example

{{example:types.String.lstrip}}
