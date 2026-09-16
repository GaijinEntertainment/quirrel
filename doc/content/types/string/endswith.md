---
params: [suffix]
see_also: [types.String.startswith, string.endswith]
---

Returns whether `str` ends with `suffix`.

## Parameters

- `suffix` - the suffix to look for

## Return value

`true` when the last `suffix.len()` bytes of `str` equal `suffix`, `false`
otherwise. An empty `suffix` always matches; a `suffix` longer than `str`
cannot match.

## Notes

Takes exactly one argument. Same implementation as
[string.endswith](sym:string.endswith); see the Notes on
[types.String.strip](sym:types.String.strip) for how the two forms differ
only in their reported attributes, not their behavior.

The comparison is a raw byte compare; there is no case-insensitive form.

## Example

{{example:types.String.endswith}}
