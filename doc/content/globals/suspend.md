---
see_also: [newthread]
---

Pauses the running thread and hands its arguments to whoever resumes it.

## Parameters

- `...` - values passed to the caller of `thread.call` or `thread.wakeup`

## Return value

The arguments given to the `thread.wakeup` call that resumes this thread.

## Errors

Throws `cannot suspend through native calls/metamethods` when a native
function is on the stack between the thread's entry point and this call - for
example inside an `each` or `sort` callback, or a metamethod.

## Notes

Only meaningful on a thread created with `newthread`, while that thread is
running after a `thread.call` or `thread.wakeup`: the value passed to
`suspend` becomes that call's return value, and the thread stays paused until
`thread.wakeup` is called on it, at which point `suspend` returns `wakeup`'s
own arguments.

Calling `suspend` directly in the main script, outside any such thread,
throws `cannot suspend the root vm`, which a `try`/`catch` can handle like any
other error.

## Example

{{example:suspend}}
