---
see_also: [types.WeakRef.ref, types.Array.is_frozen]
---

Returns a weak reference to this array.

## Return value

A new [weakref](sym:types.WeakRef) value. It does not keep the array alive;
once nothing else references the array, the weak reference stops resolving.

## Notes

Call [ref](sym:types.WeakRef.ref) on the result to get the array back.

## Example

{{example:types.Array.weakref}}
