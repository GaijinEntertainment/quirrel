---
see_also: [newthread, types.Thread.wakeup, types.Thread.getstatus]
---

Starts the thread's function, passing every argument straight through to it.

## Parameters

- `...` - passed to the thread's function as its own arguments

## Return value

Whatever the thread's function returns, or the value the function passes to
`suspend` the first time it suspends.

## Errors

Whatever the thread's function throws, propagated unchanged.

## Notes

`call` always forwards every argument as-is; unlike `types.Function.call`,
none of them is taken as an override for `this` - the function runs with the
thread's own root table as `this`, as a plain call would.

`call` does not check the thread's current state before running it. Calling
it again on a thread that is `"suspended"` or `"running"` does not raise a
clean state error the way `wakeup` does: it reads a leftover value from the
thread's own stack as if it were the function to call, and raises whatever
generic `attempt to call '<type>'` error that value happens to produce (for
example `attempt to call 'string'`). Check `getstatus` first.

Calling `call` again once the thread has gone back to `"idle"` - after its
function returned or threw - restarts the same function from the top with
the new arguments; a thread is reusable, not one-shot.

An exception that reaches the top of the thread's function uncaught is still
delivered to the caller of `call` as a normal, catchable error - but before
that, if the host installed a runtime error reporter (as the command-line
tool does), it reports the failure straight from inside the thread, dumping
that thread's own callstack and source path to the error stream. A
`try`/`catch` around `call` does not prevent this: only a `try`/`catch`
inside the thread's own function can.

## Example

{{example:types.Thread.call}}
