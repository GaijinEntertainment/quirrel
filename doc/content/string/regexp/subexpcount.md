---
see_also: [string.regexp.capture]
---

Returns how many sub-expressions the pattern has.

## Return value

The number of capturing groups in the pattern, plus one for the whole match:
`1` when the pattern has no capturing group, one more for each `(...)`
group written in it. `(?:...)` groups are not capturing and do not count.
This is a static property of the compiled pattern - it needs no string to
match against, and is the same before and after any call to
[match](sym:string.regexp.match), [search](sym:string.regexp.search) or
[capture](sym:string.regexp.capture).

## Notes

`subexpcount()` always equals `capture(str).len()` once `str` matches.

## Example

{{example:string.regexp.subexpcount}}
