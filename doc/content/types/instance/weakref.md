---
see_also: [types.Class.weakref, types.WeakRef.ref]
---

Returns a weak reference to the instance.

## Return value

A `weakref` value. Its `ref()` method gives back this same instance as long as
something else still keeps it alive, or `null` once it has been collected.

## Notes

Holding a weak reference to an instance does not keep it, or the class it
points back to through `getclass()`, alive.

## Example

{{example:types.Instance.weakref}}
