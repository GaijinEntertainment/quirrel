---
see_also: [string.regexp.capture, string.regexp.match]
---

Finds the first match of the pattern in `str` at or after `start`, and
returns its span.

## Parameters

- `str` - the string to search
- `start` - the index to start searching from; `0` when omitted

## Return value

A table with `begin` and `end`, the bounds of the first match, counted from
the start of `str` and not from `start`. `end` is exclusive, so
`str.slice(m.begin, m.end)` is the matched text. `null` when there is no
match at or after `start`.

## Errors

Throws `start index out of range` when `start` is negative or greater than
`str.len()`. `start` equal to `str.len()` is valid; it searches an empty
remainder and finds nothing unless the pattern can match empty.

Throws `regexp match aborted: pattern too complex for this input` instead of
returning `null` when the search exceeds the engine's backtracking budget,
see [match](sym:string.regexp.match) for when that happens. A `null` result
never hides an aborted search.

## Notes

`start` also moves where `^` and a word boundary (`\b`) consider the string
to begin: both test relative to `start`, not to index `0`, so an anchored
pattern can match starting exactly at `start` even though the character
right before it in `str` says otherwise. `$` still means the true end of
`str`.

Among candidate matches, `search` returns the leftmost one; among those
starting at the same position, quantifiers try the longest match first and
alternation tries its branches in the order written.

## Example

{{example:string.regexp.search}}
