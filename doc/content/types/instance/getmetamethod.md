---
params: [name]
see_also: [types.Class.getmetamethod, types.Instance.getclass]
---

Looks up one of this instance's class's metamethods by name, without
triggering it.

## Parameters

- `name` - a metamethod name, underscore included (`"_add"`, `"_tostring"`, ...)

## Return value

The closure installed for that metamethod on the instance's class, or `null`
if the class does not define it. Equivalent to `getclass().getmetamethod(name)`.

## Errors

Throws `Unknown metamethod` if `name` is not one of the metamethod names the VM
recognizes: `_add`, `_sub`, `_mul`, `_div`, `_unm`, `_modulo`, `_set`, `_get`,
`_typeof`, `_nexti`, `_cmp`, `_call`, `_cloned`, `_newslot`, `_delslot`,
`_tostring`, `_lock`.

## Example

{{example:types.Instance.getmetamethod}}
