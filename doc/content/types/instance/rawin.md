---
params: [key]
see_also: [types.Instance.rawget, types.Instance.hasindex, types.Class.rawin]
---

Tests whether `key` names a field or method reachable on this instance.

## Parameters

- `key` - the field or method name to test for

## Return value

`true` if `key` is one of the instance's own fields or one of its class's
methods, `false` otherwise. Same result as `hasindex(key)`.

## Notes

Marked `pure`. Does not consult a `_get` metamethod, so it reports `false` for
a key `_get` would otherwise answer for.

## Example

{{example:types.Instance.rawin}}
