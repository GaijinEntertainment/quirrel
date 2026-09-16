---
see_also: [types.Instance.getmetamethod, types.Class.getfuncinfos, types.Function.getfuncinfos]
---

Describes this instance's class's `_call` metamethod, the same way
`function.getfuncinfos` describes a plain function.

## Return value

`null` if the instance's class defines no `_call` metamethod (so the instance
cannot be called like a function). Otherwise the introspection table for that
metamethod closure: `name`, `parameters`, `defparams`, `required_params`,
`varargs`, `native`, and the rest.

## Notes

Equivalent to `getclass().getfuncinfos()`. Not about `constructor`, which is a
plain method and never consulted here.

## Example

{{example:types.Instance.getfuncinfos}}
