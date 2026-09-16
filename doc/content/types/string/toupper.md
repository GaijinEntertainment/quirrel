---
params: [start, end]
see_also: [types.String.tolower, types.String.slice]
---

Returns a copy of `str` with the ASCII letters in `[start, end)` uppercased.

## Parameters

- `start` - index of the first byte to convert; defaults to 0
- `end` - index one past the last byte to convert; defaults to `str.len()`
- `...` - not a real parameter; see Notes

## Return value

A copy of `str` the same length as the original, with every byte in
`[start, end)` that is `a`-`z` replaced by its uppercase letter. Bytes
outside the range, and bytes inside it that are not `a`-`z` (including any
byte of a multi-byte character), are copied unchanged. With no arguments the
whole string is uppercased.

## Errors

Throws `wrong indexes` when `end < start` after negative indices are
resolved (see [types.String.tolower](sym:types.String.tolower)). Throws
`slice out of range` when `end > str.len()` or the resolved `start < 0`.

## Notes

Takes 0, 1 or 2 arguments, not the variadic tail the VM's dump implies. See
the Notes on [types.String.tolower](sym:types.String.tolower) for why an
out-of-range range throws here but clamps for
[types.String.slice](sym:types.String.slice), and for the ASCII-only scope of
the conversion.

## Example

{{example:types.String.toupper}}
