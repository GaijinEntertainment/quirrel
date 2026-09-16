---
see_also: [types.Table.weakref, types.Function.clone]
---

Returns a weak reference to the closure.

## Return value

A `weakref` value. Calling its `ref()` method gives the closure back for as
long as something else still holds a strong reference to it; once the
closure's refcount drops to zero, `ref()` returns `null` instead.

## Notes

Takes no arguments. This is a weak reference to the closure value itself -
unrelated to the weak reference `bindenv` keeps on the environment it binds
inside a closure.

## Example

{{example:types.Function.weakref}}
