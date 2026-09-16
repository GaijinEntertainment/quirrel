---
params: [prefix]
see_also: [types.String.endswith, string.startswith]
---

Returns whether `str` begins with `prefix`.

## Parameters

- `prefix` - the prefix to look for

## Return value

`true` when the first `prefix.len()` bytes of `str` equal `prefix`, `false`
otherwise. An empty `prefix` always matches; a `prefix` longer than `str`
cannot match.

## Notes

Takes exactly one argument. Same implementation as
[string.startswith](sym:string.startswith), reached with `str` and `prefix`
swapped between an explicit first argument and the method receiver; see the
Notes on [types.String.strip](sym:types.String.strip) for how the two forms
differ only in their reported attributes, not their behavior.

The comparison is a raw byte compare, so it works the same on a multi-byte
`prefix` as on any other bytes; there is no case-insensitive form.

## Example

{{example:types.String.startswith}}
