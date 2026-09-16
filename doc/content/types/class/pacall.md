---
params: [args]
see_also: [types.Class.acall, types.Class.pcall, types.Function.pacall]
---

Same as `acall`, except a failure does not invoke the VM's registered error
callback.

## Parameters

- `args` - array of constructor arguments; `args[0]` must be present but its
  value is ignored, as in `acall`

## Return value

The new instance.

## Errors

Throws a parameter type error if `args` is not an array. Takes exactly one
argument beyond the receiver. A constructor failure still raises a normal,
catchable script error - "protected" here only means the host's error hook is
skipped, not that the error is swallowed; see Notes.

## Notes

The `p` in `pcall`/`pacall` stands for "protected call", but in Quirrel that
protection is aimed at the *host embedding the VM*, not at the calling script:
a failed `pacall` still throws, and an uncaught throw still stops the script.
Wrap the call in `try`/`catch` if the script itself needs to recover.

## Example

{{example:types.Class.pacall}}
