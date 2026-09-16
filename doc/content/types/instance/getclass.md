---
see_also: [types.classof, types.Class.getbase, types.Instance.getfuncinfos]
---

Returns the class this instance was constructed from.

## Return value

The exact class used to create the instance - never a base class, even if that
class was declared with `class Derived(Base) { ... }`. Same value `classof`
returns for an instance.

## Notes

Marked `pure`. Use `instanceof` to test against a class including its bases;
`getclass()` only ever names the single, most-derived class.

## Example

{{example:types.Instance.getclass}}
