---
params: [start, end]
see_also: [types.String.toupper, types.String.slice]
---

Returns a copy of `str` with the ASCII letters in `[start, end)` lowercased.

## Parameters

- `start` - index of the first byte to convert; defaults to 0
- `end` - index one past the last byte to convert; defaults to `str.len()`
- `...` - not a real parameter; see Notes

## Return value

A copy of `str` the same length as the original, with every byte in
`[start, end)` that is `A`-`Z` replaced by its lowercase letter. Bytes
outside the range, and bytes inside it that are not `A`-`Z` (including any
byte of a multi-byte character), are copied unchanged. With no arguments the
whole string is lowercased.

## Errors

Throws `wrong indexes` when `end < start` after negative indices (counted
from the end, as for [types.String.slice](sym:types.String.slice)) are
resolved. Throws `slice out of range` when `end > str.len()` or the resolved
`start < 0`.

Unlike [types.String.slice](sym:types.String.slice), an out-of-range or
inverted `[start, end)` throws here instead of clamping - despite both
functions reading their range with the same helper.

## Notes

Takes 0, 1 or 2 arguments, not the variadic tail the VM's dump implies (it
shows `tolower([arg1: number, arg2: number], ...)` because this binding
carries no declaration string).

Case conversion only ever touches the ASCII bytes `A`-`Z`; there is no
locale and no Unicode case folding, so a non-ASCII byte is never altered.

## Example

{{example:types.String.tolower}}
