---
params: [start, end]
see_also: [types.String.indexof, types.String.tolower, types.Array.slice]
---

Returns the bytes of `str` from `start` up to, but not including, `end`.

## Parameters

- `start` - index of the first byte to keep; defaults to 0
- `end` - index one past the last byte to keep; defaults to `str.len()`
- `...` - not a real parameter; see Notes

## Return value

A new string holding `str[start:end]`. A negative `start` or `end` counts
from the end of `str`, the same as a negative index anywhere else in
Quirrel: `-1` means the last byte.

Both indices are then clamped into `[0, str.len()]`; an `end` at or before
`start` after clamping gives `""`. No combination of arguments throws: an
index far past either end of `str`, or an inverted range, silently clamps
instead. Contrast [types.String.tolower](sym:types.String.tolower) and
[types.String.toupper](sym:types.String.toupper), which take the same kind of
range but throw instead of clamping.

`str` is a byte buffer, so a `start` or `end` that falls inside a multi-byte
character splits it; the two halves are no longer valid UTF-8 on their own.

## Notes

Takes 0, 1 or 2 arguments, not the variadic tail the VM's dump implies (it
shows `slice([arg1: number, arg2: number], ...)` because this binding carries
no declaration string). A third or later argument is silently ignored.

## Example

{{example:types.String.slice}}
