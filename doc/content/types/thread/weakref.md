---
see_also: [types.Thread.getstatus, types.Table.weakref]
---

Returns a weak reference to the thread.

## Return value

A `weakref` value. Calling its `ref()` method gives the thread back for as
long as something else still holds a strong reference to it; once the
thread's refcount drops to zero, `ref()` returns `null` instead.

## Notes

Takes no arguments. A suspended thread's own stack keeps its local variables
alive independently of this; dropping every strong reference to a suspended
thread while nothing ever wakes it up again lets the thread itself, and
everything only it still referenced, be collected.

## Example

{{example:types.Thread.weakref}}
