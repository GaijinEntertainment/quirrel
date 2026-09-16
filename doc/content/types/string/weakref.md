---
see_also: [types.String.clone, types.classof]
---

Returns a weak reference to `str`.

## Return value

A `weakref` pointing at `str`.

## Notes

Takes no arguments; the VM reports the true arity here.

The VM's signature dump shows the receiver as
`(table|userdata|instance|class|null).weakref()`: this binding carries no
declared parameter types, so the dump falls back to a generic receiver list
used for every such binding, not the real one - the receiver is a string
here, as the name `types.String.weakref` implies.

Every built-in type shares this same type method (a table, an array, an
instance, and so on all get their own `weakref()` the same way), so a weak
reference to a string works the same as for any other value.

## Example

{{example:types.String.weakref}}
