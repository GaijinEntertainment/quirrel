---
see_also: [types.WeakRef.ref, types.Table.tostring, types.Integer.tostring]
---

Returns a string naming the type and identity of this `weakref` object
itself - not of the value it points at.

## Return value

A `string` shaped like `(weakref : 0x...)`, the same generic format
[`types.Table.tostring`](sym:types.Table.tostring) uses, since `weakref` has
no `_tostring` metamethod of its own to override it.

## Errors

The address in the result changes from run to run and between processes:
never compare or print it verbatim in code whose output must stay stable,
only match against the fixed part of the string, such as
`s.indexof("weakref") != null`.

## Notes

Takes no arguments. To see the *pointed-to* value's own string form instead,
dereference first: `wr.ref().tostring()`.

## Example

{{example:types.WeakRef.tostring}}
