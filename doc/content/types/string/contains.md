---
params: [substr, start]
see_also: [types.String.indexof, types.String.startswith]
---

Returns whether `str` contains `substr`.

## Parameters

- `substr` - the substring to look for; must not be empty
- `start` - byte index to start searching from; defaults to 0
- `...` - not a real parameter; see Notes

## Return value

`true` if `substr` occurs at or after `start`, `false` otherwise - always a
`bool`, never `null`. This is the difference from
[types.String.indexof](sym:types.String.indexof): the same scan, but `contains`
never reports "not found" as anything other than `false`. A negative or
out-of-range `start` gives `false`, not a wraparound search.

## Errors

Throws `empty substring` when `substr` is `""`.

## Notes

Takes 1 or 2 arguments: `start` is optional. A third argument is silently
ignored.

## Example

{{example:types.String.contains}}
