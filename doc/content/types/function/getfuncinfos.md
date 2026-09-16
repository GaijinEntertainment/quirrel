---
see_also: [debug.get_function_info_table, types.Function.getfreevar]
---

Returns a table describing the closure: its name, parameters, and other
introspection data the VM tracks for it.

## Return value

A table with these keys:

- `native` - `true` for a native closure, `false` for a script closure
- `pure`, `nodiscard`, `fastcall` - the closure's attributes
- `name` - the closure's name, or `null` if it was never given one
- `freevars` - how many free variables it captured (see `getfreevar`)
- `src`, `line` - source file and first line; `null` for a native closure
- `parameters` - an array of parameter names, starting with `"this"`
- `defparams` - default values, one per parameter that has one
- `required_params` - how many leading entries of `parameters` (including
  `this`) have no default
- `varargs` - `true` when the closure ends in `...`
- `typecheck`, `return_type_mask`, `varargs_type_mask` - type masks for the
  parameters, the return value, and the vararg tail; see `debug.type_mask_to_string`
- `doc` - the docstring, or `null`
- `paramscheck` - a native closure only: the raw registration count `call`
  checks against (see `debug.get_function_info_table` for how it is turned
  into a plain required-argument count)

## Notes

`this` is always `parameters[0]`, so `required_params` and the length of
`parameters` both count it; `debug.get_function_info_table`'s `requiredArgs`
and `argNames` do not, so
`f.getfuncinfos().required_params` is one higher than
`get_function_info_table(f).requiredArgs` for the same `f`.

## Example

{{example:types.Function.getfuncinfos}}
