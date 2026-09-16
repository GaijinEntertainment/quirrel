---
see_also: [async.Future.resolve, async.Future.getValue, async.Future.markHandled]
---

Settles a pending future as faulted with `value`.

## Parameters

- `value` - the value to fault with; `null` when omitted

## Errors

Same two guards as [resolve](sym:async.Future.resolve): rejecting with
itself throws `cannot reject a future with itself`, and rejecting a
task-future throws `cannot reject the future returned by an async
function`.

## Notes

Ignored with no error on a future that has already settled - the same
idempotency as `resolve`.

A faulted future that nothing awaits is reported through the
unhandled-fault path the next time the host pumps the runtime, the same as
an uncaught `throw` inside an `async` function. Consume it with `await`, or
acknowledge it with [markHandled](sym:async.Future.markHandled) without
consuming it, to stop that report. The call site of `reject` is captured,
so an abandoned future's report carries an `ERROR TRACE` pointing at where
`reject` was called - unlike a `Future` faulted from native code, which
carries no such trace.

## Example

{{example:async.Future.reject}}
