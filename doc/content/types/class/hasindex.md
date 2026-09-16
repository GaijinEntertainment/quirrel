---
params: [key]
see_also: [types.Class.rawin, types.Class.rawget, types.Instance.hasindex]
---

Tests whether `key` names a member of the class.

## Parameters

- `key` - the member name to test for

## Return value

`true` if the class has an own member called `key`, `false` otherwise.

## Notes

Marked `pure`. Behaves the same as `rawin` here: a class has no delegate of its
own for indexing to bypass, so there is nothing "hasindex" skips that plain
lookup would not. A derived class's inherited members count too, since they are
copied into its own member table when the class is declared.

## Example

{{example:types.Class.hasindex}}
