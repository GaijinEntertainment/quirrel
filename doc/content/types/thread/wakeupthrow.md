---
params: [value, rethrow]
see_also: [types.Thread.wakeup, suspend]
---

Resumes a suspended thread by throwing `value` at its `suspend` call, instead
of returning from it normally.

## Parameters

- `value` - thrown inside the thread, at the point it is suspended
- `rethrow` - when the thread does not catch `value` and dies from it,
  whether to re-throw that same error out of this call; defaults to `true`

## Return value

Whatever the thread passes to its next `suspend`, or returns, if it catches
`value` and keeps running. `null` if the thread dies from `value` and
`rethrow` is `false`.

## Errors

`value` is required: calling with no arguments throws `wrong number of
parameters passed to native closure 'wakeupthrow' (1 passed, at least 2
required)` before the thread is touched.

Throws `cannot wakeup a idle thread` or `cannot wakeup a running thread` for
the same reasons `wakeup` does - only a `"suspended"` thread can be resumed
this way.

When the thread does not catch `value` and dies from it, this call re-throws
the same error, unless `rethrow` is `false`.

## Notes

The signature shows `rethrow` as optional and ends in `...`, but nothing past
`rethrow` is ever read - a third argument is accepted and silently ignored,
the same way extra arguments are for `wakeup`.

`rethrow` only controls whether *this call* reports the error to its own
caller; it cannot silence an uncaught `value`. A thread runs on
its own stack with its own exception traps, so a `try`/`catch` around the
call to `wakeupthrow` never sees `value` unless the thread's own body
lets it escape - and if it does escape, the host's runtime error reporter
(if one is installed, as the command-line tool does) reports it straight
from there, dumping that thread's own callstack and source path to the error
stream, regardless of `rethrow`. Only a `try`/`catch` inside the thread's own
body can prevent that report. Either way the thread goes back to `"idle"`
and can be started again with `call`.

## Example

{{example:types.Thread.wakeupthrow}}
