---
see_also: [string.regexp.search, string.regexp.constructor]
---

Returns whether the pattern matches the whole of `str`.

## Parameters

- `str` - the string to test

## Return value

`true` when the pattern matches all of `str`, from the first character to
the last. `false` for an ordinary mismatch. `false` never means the match
was aborted, see Errors.

## Errors

Throws `regexp match aborted: pattern too complex for this input` instead of
returning `false` when matching exceeds the engine's backtracking budget -
for example a pattern with nested unbounded quantifiers tried against a long
non-matching input. It is a runtime safety limit, not a syntax error, so it
can happen on any pattern given a bad enough input.

## Notes

`match` is anchored at both ends. Use [search](sym:string.regexp.search) to
test whether the pattern occurs anywhere inside `str`, and
[capture](sym:string.regexp.capture) to get its capturing groups too.

## Example

{{example:string.regexp.match}}
