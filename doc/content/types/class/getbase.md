---
see_also: [types.Instance.getclass, types.classof]
---

Returns the class this class inherits from.

## Return value

The base class given in `class Derived(Base) { ... }`, or `null` for a class
with no base.

## Notes

Marked `pure`: the result depends only on which class object `getbase` is
called on, since a class's base is fixed at declaration and never changes
afterward.

## Example

{{example:types.Class.getbase}}
