---
params: [name]
see_also: [types.Instance.getmetamethod, types.Class.newmember]
---

Looks up one of the class's metamethods by name, without triggering it.

## Parameters

- `name` - a metamethod name, underscore included (`"_add"`, `"_tostring"`, ...)

## Return value

The closure installed for that metamethod, or `null` if the class does not
define it.

## Errors

Throws `Unknown metamethod` if `name` is not one of the metamethod names the VM
recognizes: `_add`, `_sub`, `_mul`, `_div`, `_unm`, `_modulo`, `_set`, `_get`,
`_typeof`, `_nexti`, `_cmp`, `_call`, `_cloned`, `_newslot`, `_delslot`,
`_tostring`, `_lock`. A recognized name that the class has not
implemented is not an error; it returns `null` instead.

## Notes

Calling `Instance.getmetamethod(name)` on an instance reads from that
instance's own class, so it agrees with calling the same method on the class
directly.

## Example

{{example:types.Class.getmetamethod}}
