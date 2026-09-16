---
see_also: [types.Instance.weakref, types.WeakRef.ref]
---

Returns a weak reference to the class.

## Return value

A `weakref` value. Its `ref()` method gives back this same class object as
long as something else still keeps it alive, or `null` once it has been
collected.

## Notes

Holding a weak reference to a class does not keep it, or the instances still
pointing at it as their `getclass()`, alive.

## Example

{{example:types.Class.weakref}}
