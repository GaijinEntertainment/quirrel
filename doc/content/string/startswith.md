---
see_also: [string.endswith]
---

Returns whether `str` begins with `prefix`.

## Parameters

- `str` - the string to test
- `prefix` - the prefix to look for

## Return value

`true` when the first `prefix.len()` characters of `str` equal `prefix`,
`false` otherwise.

## Notes

An empty `prefix` always matches: every string starts with the empty string.
A `prefix` longer than `str` cannot match and gives `false` without reading
past the end of `str`.

The comparison is a raw byte compare; there is no case-insensitive form.

## Example

{{example:string.startswith}}
