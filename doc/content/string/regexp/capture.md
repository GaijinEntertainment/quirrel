---
see_also: [string.regexp.search, string.regexp.subexpcount]
---

Finds the first match of the pattern in `str` at or after `start`, and
returns the whole match plus every capturing group as an array of spans.

## Parameters

- `str` - the string to search
- `start` - the index to start searching from; `0` when omitted

## Return value

An array of tables shaped like [search](sym:string.regexp.search)'s result
(`begin` and `end`). Index `0` is always the whole match. Index `i`, for `i`
from `1`, is the `i`-th capturing group counted by where its `(` opens, not
where it closes: in `((a)b)` the outer group is index `1` and the inner one
is index `2`. A group that took no part in the match - for example one
inside an alternative that did not run - is reported as the span `0..0`,
relative to the very start of `str`, not `null` and not relative to `start`
or to the match. `null` when there is no match at or after `start`.

## Errors

The same as [search](sym:string.regexp.search): `start index out of range`
for a `start` outside `[0, str.len()]`, and `regexp match aborted: pattern
too complex for this input` instead of returning `null` when the search
exceeds the backtracking budget.

## Notes

[subexpcount](sym:string.regexp.subexpcount) gives the length of the
returned array in advance, once `str` matches: one for the whole match, plus
one per capturing group. `(?:...)` groups are not capturing and add nothing
to either.

## Example

{{example:string.regexp.capture}}
