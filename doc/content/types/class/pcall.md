---
see_also: [types.Class.call, types.Class.pacall, types.Function.pcall]
---

Same as `call`, except a failure does not invoke the VM's registered error
callback.

## Parameters

- `...` - the constructor's arguments, with the same required-but-ignored
  leading value as `call` (see its Notes)

## Return value

The new instance.

## Errors

A constructor failure still raises a normal, catchable script error; see
`call`'s Notes for the leading-argument rule and this page's Notes for what
"protected" does and does not mean here.

## Notes

The `p` in `pcall` stands for "protected call", but in Quirrel that protection
is aimed at the *host embedding the VM*, not at the calling script: a failed
`pcall` still throws, and an uncaught throw still stops the script. Wrap the
call in `try`/`catch` if the script itself needs to recover.

## Example

{{example:types.Class.pcall}}
