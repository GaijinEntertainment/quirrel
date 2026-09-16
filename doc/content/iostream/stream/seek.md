---
see_also: [iostream.blob.seek, iostream.stream.tell, iostream.stream.len]
---

Moves the cursor to `offset`, relative to `origin`, and reports whether the
move landed inside the stream.

## Parameters

- `offset` - how far to move, in bytes
- `origin` - `'b'` (begin), `'c'` (current) or `'e'` (end); defaults to `'b'`

## Return value

`0` when the target position is within `[0, len()]`, and the cursor is now
there. `-1` when it falls outside that range; the cursor is left where it
was, so a failed `seek` never leaves the stream half-moved.

## Errors

Throws `invalid origin` when `origin` is not `'b'`, `'c'` or `'e'`.

## Notes

The same method works on a `file`: `'b'`, `'c'` and `'e'` mean the same three
origins, and the same `0`/`-1` convention reports success or an out-of-range
target.

## Example

{{example:iostream.stream.seek}}
