---
see_also: [suspend, types.Thread.wakeupthrow, types.Thread.getstatus]
---

Resumes a suspended thread, passing one value back to its `suspend` call.

## Parameters

- `...` - at most the first extra argument is used; the rest are accepted
  and silently ignored

## Return value

Whatever the thread passes to its next `suspend`, or returns, or `null` when
no value was given for either.

## Errors

Throws `cannot wakeup a idle thread` when the thread has not been started (or
already ran to completion) and `cannot wakeup a running thread` when it is
currently executing - only a `"suspended"` thread can be woken up.

## Notes

The signature looks fully variadic, the same shape as `types.Function.call`,
but it is not: `t.wakeup(a, b, c)` delivers only `a` to the thread's
`suspend` call, the same as `t.wakeup(a)` - `b` and `c` are read, not
checked, and thrown away. Calling with no argument resumes with
`null`, the same as a bare `suspend()` with nothing passed to it.

An exception that reaches the top of the thread's function uncaught after
this call is still delivered to the caller of `wakeup` as a normal,
catchable error, but a host-installed runtime error reporter also reports it
straight from inside the thread first - see the Notes on `types.Thread.call`
for details.

## Example

{{example:types.Thread.wakeup}}
