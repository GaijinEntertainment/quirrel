---
params: [args]
see_also: [types.Function.call, types.Function.pacall]
---

Calls the closure with the arguments taken from an array: `args[0]` becomes
`this`, and the rest of `args` becomes the closure's own parameters.

## Parameters

- `args` - an array; its first element is `this`, the rest are the arguments

## Return value

Whatever the call returns.

## Errors

Throws `parameter 1 of 'acall' has an invalid type` when `args` is not an
array. Whatever the call itself throws is otherwise propagated unchanged.

## Notes

Unlike `call`, `acall` takes exactly one explicit argument - the array - so
`f.acall(a, b)` fails with a parameter-count error rather than treating `b`
as anything. It exists for the case where the argument list is already an
array (built up in a loop, say) instead of a literal comma list.

## Example

{{example:types.Function.acall}}
