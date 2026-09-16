---
see_also: [types.String.strip, types.String.lstrip, string.rstrip]
---

Returns `str` with white space removed from the end only.

## Return value

A new string with the trailing white space removed; leading white space, if
any, is kept. See [string.rstrip](sym:string.rstrip) for which bytes
count as white space.

## Notes

Takes no arguments. Same implementation as [string.rstrip](sym:string.rstrip);
see the Notes on [types.String.strip](sym:types.String.strip) for how the two
forms differ only in their reported attributes, not their behavior.

## Example

{{example:types.String.rstrip}}
