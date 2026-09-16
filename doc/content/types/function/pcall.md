---
see_also: [types.Function.call, types.Function.pacall]
---

Calls the closure like `call` does, using `env` as its `this` and forwarding
the rest of the arguments as its own parameters.

## Parameters

- `...` - the first argument becomes `this` inside the call; the rest are
  forwarded as the closure's own arguments

## Return value

Whatever the call returns.

## Errors

Whatever the call itself throws, propagated unchanged - a `try`/`catch`
around `pcall` catches it the same as it would around `call`.

## Notes

`pcall` never tail-calls, unlike `call`: every `pcall` keeps its own frame on
the stack until the callee returns, so a deep chain of `f.pcall(...)`
recursion exhausts the native call depth (`Native stack overflow`) much
sooner than the same recursion written with `call`.

The two also differ once an error escapes uncaught: a host that installs a
runtime error reporter (as the command-line tool does, to print a callstack)
sees the callee's own frame for an unhandled error raised through `call`, but
not through `pcall` - the report, if the host ever produces one, comes from
whichever frame the error next escapes into. This has no effect on
`try`/`catch`, which behaves identically either way.

## Example

{{example:types.Function.pcall}}
