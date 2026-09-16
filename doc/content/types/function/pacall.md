---
params: [args]
see_also: [types.Function.acall, types.Function.pcall]
---

Calls the closure like `acall` does, taking `this` and the arguments from an
array: `args[0]` becomes `this`, and the rest of `args` becomes the closure's
own parameters.

## Parameters

- `args` - an array; its first element is `this`, the rest are the arguments

## Return value

Whatever the call returns.

## Errors

Throws `parameter 1 of 'pacall' has an invalid type` when `args` is not an
array. Whatever the call itself throws is otherwise propagated unchanged.

## Notes

`pacall` is to `acall` what `pcall` is to `call`: the calling convention is
identical, but an unhandled error raised inside the call does not show this
frame to a host-installed runtime error reporter (see the Notes on `pcall`
for what that does and does not change).

## Example

{{example:types.Function.pacall}}
