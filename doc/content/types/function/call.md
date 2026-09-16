---
see_also: [types.Function.pcall, types.Function.acall, types.Function.bindenv]
---

Calls the closure, using `env` as its `this` and forwarding the rest of the
arguments as its own parameters.

## Parameters

- `...` - the first argument becomes `this` inside the call; the rest are
  forwarded as the closure's own arguments

## Return value

Whatever the call returns.

## Errors

Whatever the call itself throws, propagated unchanged.

## Notes

`env` is not optional: `f.call()` fails with the arity error of `f` itself
(zero arguments reached it, including the missing `this`), not an error from
`call`. Anything beyond `env` is optional only in the sense that `f` may
default or ignore it - `call` forwards what it is given.

When `f` is a non-native, non-generator closure, `call` tail-calls into it:
the frame for this call does not stay on the stack, so `f.call(env, ...)` in
a function's own tail position recurses as cheaply as a direct call. `pcall`
never does this. Calling a generator-producing function through `call`
behaves like calling it directly: it returns a new suspended
generator without running the body.

## Example

{{example:types.Function.call}}
