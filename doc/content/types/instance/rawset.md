---
params: [key, value]
see_also: [types.Instance.rawget, types.Instance.swap, types.Class.newmember]
---

Replaces the value stored at an existing field of this instance, without
consulting a `_newslot` metamethod.

## Parameters

- `key` - the field name to update
- `value` - the value to store there

## Return value

The instance itself, so calls can be chained.

## Errors

Throws `the index '<key>' (type='<type>') does not exist` if `key` is not
already a field of the instance. Unlike a table or an unlocked class, an
instance can never gain a new slot this way - the language reference
states instance members cannot be removed, and the same restriction covers
adding one. Attempting to rawset a method name fails the same way: a method is
not a per-instance value slot to overwrite. On a frozen reference, throws
`Cannot modify immutable object`.

## Notes

Use `rawset` when the instance's class defines a `_newslot` metamethod and the
plain (non-raw) assignment should not trigger it.

## Example

{{example:types.Instance.rawset}}
