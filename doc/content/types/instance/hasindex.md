---
params: [key]
see_also: [types.Instance.rawin, types.Instance.rawget, types.Class.hasindex]
---

Tests whether `key` names a field or method reachable on this instance.

## Parameters

- `key` - the member name to test for

## Return value

`true` if `key` is one of the instance's own fields or one of its class's
methods, `false` otherwise.

## Notes

Marked `pure`. Unlike a table, an instance's methods live on its class rather
than in a separate delegate, and `hasindex`/`rawget`/`rawin` all see them
directly: they are not something "raw" access skips here.

## Example

{{example:types.Instance.hasindex}}
