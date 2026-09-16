---
see_also: [types.Class.getmetamethod, types.Instance.getfuncinfos, types.Function.getfuncinfos, debug.get_function_info_table]
---

Describes the class's `_call` metamethod, the same way `function.getfuncinfos`
describes a plain function.

## Return value

`null` if the class defines no `_call` metamethod. Otherwise the same
introspection table `debug.get_function_info_table` would build for that
metamethod closure: `name`, `parameters`, `defparams`, `required_params`,
`varargs`, `native`, and the rest.

## Notes

This is about `_call`, the operator that makes an *instance* of the class
callable like a function - not about `constructor`, which is a plain method and
never consulted here. A class with no `_call` gets `null` even though every
class has a constructor.

## Example

{{example:types.Class.getfuncinfos}}
