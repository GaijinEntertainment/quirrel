---
see_also: [string.startswith]
---

Returns whether `str` ends with `suffix`.

## Parameters

- `str` - the string to test
- `suffix` - the suffix to look for

## Return value

`true` when the last `suffix.len()` characters of `str` equal `suffix`,
`false` otherwise.

## Notes

An empty `suffix` always matches: every string ends with the empty string.
A `suffix` longer than `str` cannot match and gives `false` without reading
past the start of `str`.

The comparison is a raw byte compare; there is no case-insensitive form.

## Example

{{example:string.endswith}}
