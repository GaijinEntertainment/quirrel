---
see_also: [types.String.slice, types.String.hasindex]
---

Returns the length of `str` in bytes.

## Return value

The number of bytes in `str`. A Quirrel string is a byte buffer, not a
sequence of code points: a 2-byte UTF-8 character counts as 2, not 1.

## Notes

Takes no arguments; the VM reports the true arity here (`len(): any`), so
there is nothing for a page to override.

## Example

{{example:types.String.len}}
