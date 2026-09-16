---
params: [key]
see_also: [types.Instance.rawin, types.Instance.rawset, types.Class.rawget]
---

Reads the value stored at `key` on this instance, without consulting a `_get`
metamethod.

## Parameters

- `key` - the field or method name to look up

## Return value

The field's current value, or the method closure for `key`, read directly from
the instance's class.

## Errors

Throws `the index doesn't exist` when `key` is neither an own field nor a
method of the instance's class; check with `rawin` first to avoid the throw.

## Notes

Unlike a table's delegate, an instance's methods are not something `rawget`
skips: they come from the class's own member layout, not from a `_get`
fallback, so `rawget` sees them exactly as plain indexing would. What `rawget`
does skip is a `_get` metamethod the class defines for keys that are not real
members.

That skip only applies to the `key` argument, not to resolving the name
`rawget` itself: a `_get` that returns a value for every key, instead of
`throw null` for the ones it does not recognize, shadows `someInstance.rawget`
(and every other built-in type-method) before the call ever happens, because
plain `.` access tries `_get` before falling back to the type-methods table.
`someInstance.$rawget(key)` bypasses `_get` for the method name and always
reaches this method; see
[the .$ type-method operator](page:language/operators#the-type-method-operator).
A well-behaved `_get` should `throw null` for keys it does not handle.

## Example

{{example:types.Instance.rawget}}
