---
see_also: [string.lstrip, string.rstrip]
---

Returns `str` with white-space characters removed from both ends.

## Parameters

- `str` - the string to trim

## Return value

A new string with the leading and trailing white space removed. If `str` is
all white space, or empty, the result is an empty string.

## Notes

White space here is exactly space, tab, newline, vertical tab, form feed and
carriage return - the ASCII set the VM checks byte by byte. A non-ASCII byte,
or any other control character, is not white space and is kept.

`strip`, [lstrip](sym:string.lstrip), [rstrip](sym:string.rstrip),
[split_by_chars](sym:string.split_by_chars), [escape](sym:string.escape),
[startswith](sym:string.startswith) and [endswith](sym:string.endswith) are
also available as methods on every string, with the same behavior:
`"  hi  ".strip()` gives the same result as `strip("  hi  ")`.

## Example

{{example:string.strip}}
