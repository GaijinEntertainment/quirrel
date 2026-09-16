---
params: [key]
see_also: [types.Class.rawget, types.Class.hasindex, types.Instance.rawin]
---

Tests whether `key` names a member of the class.

## Parameters

- `key` - the member name to test for

## Return value

`true` if the class has an own member called `key`, `false` otherwise. Same
result as `hasindex(key)`.

## Notes

Marked `pure`. Inherited members count too, since a derived class copies every
member of its base into its own table when it is declared.

## Example

{{example:types.Class.rawin}}
