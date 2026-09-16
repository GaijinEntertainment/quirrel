## What a thread is

A thread is a cooperative coroutine, not an operating system thread. Nothing runs
in parallel and nothing needs a lock: a thread runs only after a caller has resumed
it, and control returns to that caller when the thread suspends.

What separates a thread from a [generator](sym:types.Generator) is the stack. A
generator yields from its own body and nowhere else, so a helper it calls cannot
pause it. A thread has an execution stack of its own, so [suspend](sym:suspend)
works from any depth: a function three calls down can suspend the thread, and
resuming it continues from that point. A thread also carries its own error
handler, so a failure inside it does not have to be handled by the same policy as
the code that started it.

## The handshake

Values pass in both directions, and each side reads them at a different place:

- [call](sym:types.Thread.call) starts the thread; its arguments become the
  parameters of the thread function.
- `suspend(x)` pauses the thread. `x` becomes the return value of the `call` or
  [wakeup](sym:types.Thread.wakeup) that was waiting.
- `wakeup(y)` resumes it. `y` becomes the return value of the `suspend` that paused
  it.
- When the thread function returns, that value is the return value of the last
  `wakeup`.

So a resumed thread reads its input from `suspend`'s result, not from a parameter,
and the caller reads the thread's output from `wakeup`'s result. When this rule is
forgotten, the values seem to arrive one step late.

{{example:types/thread-handshake}}

## Status

[getstatus](sym:types.Thread.getstatus) reports `"idle"` before the first `call`
and again after the thread function returns, `"suspended"` while it waits, and
`"running"` when asked from inside the thread itself. A thread that ended cannot be
restarted; make a new one with [newthread](sym:newthread).

## Failure

An unhandled throw inside a thread reaches the caller that resumed it, so `call` and
`wakeup` can both throw and can be wrapped in `try`. It passes through the VM error
handler on the way, which prints the message and the thread's own call stack, so a
failing thread still prints to the log even when the caller catches it. A thread that
failed is left `"idle"`, the same as if it had returned.

[wakeupthrow](sym:types.Thread.wakeupthrow) is the counterpart of `wakeup`: instead of
returning a value to `suspend`, it raises one there, so the thread's own `try`
blocks see it. This is how a caller cancels a suspended thread from outside.

`suspend` called on the root VM rather than inside a thread throws
`cannot suspend the root vm`.
