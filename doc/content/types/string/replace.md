---
params: [from, to]
see_also: [types.String.subst, types.String.split]
---

Returns `str` with every occurrence of `from` replaced by `to`.

## Parameters

- `from` - the substring to find
- `to` - the replacement text

## Return value

A new string with every non-overlapping occurrence of `from` replaced by
`to`, scanning left to right. A replacement's own text is never rescanned for
further matches, so `"aaa".replace("a", "aa")` gives `"aaaaaa"`, not an
unbounded expansion. When `from` is `""`, `str` is returned unchanged rather
than inserting `to` between every byte.

## Notes

Takes exactly two arguments, both required; the VM reports the true arity
here.

## Example

{{example:types.String.replace}}
