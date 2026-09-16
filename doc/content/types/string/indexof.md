---
params: [substr, start]
see_also: [types.String.contains, types.String.slice, types.String.split]
---

Returns the byte index of the first occurrence of `substr` in `str`.

## Parameters

- `substr` - the substring to look for; must not be empty
- `start` - byte index to start searching from; defaults to 0
- `...` - not a real parameter; see Notes

## Return value

The byte index of the first match at or after `start`, or `null` if there is
no match - not `-1`. `start` must be within `[0, str.len())`; a negative or
out-of-range `start` also gives `null` rather than wrapping the way a
negative index does for [types.String.slice](sym:types.String.slice).

The search is a raw byte search (it calls `strstr`), so it also finds a
match that is only part of a multi-byte character, and finds `substr` at the
byte offset where its bytes occur, not at a character position.

## Errors

Throws `empty substring` when `substr` is `""`.

## Notes

Takes 1 or 2 arguments: `start` is optional. A third argument is silently
ignored.

[types.String.contains](sym:types.String.contains) runs the exact same scan
but returns a `bool` instead of an index or `null`.

## Example

{{example:types.String.indexof}}
