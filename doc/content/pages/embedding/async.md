---
title: The async runtime
group: Embedding
order: 112
summary: Installing the async runtime, and pumping it once per frame.
---

Script-side [async and await](page:language/async) is a small cooperative scheduler
whose C entry points are in `sqasync.h`. It does not drive itself, so a host has
three duties. Miss the third and every async function in the codebase starts and
never finishes.

1. **Bind it**, once per shared state, before any `async` function runs:
   `sqasync::bind(v)`.
2. **Publish the Future class** to script, through
   `SqModules::registerAsyncLib` or `sqasync::sqasync_register`.
3. **Pump it**, once per host frame: `sqasync::pump(v)`.

## Pumping

`pump` drains the inbox into the step queue and runs both, returning how many
callbacks it invoked. Its step cap bounds how long one call can take when a callback
posts another callback, so a busy frame cannot starve rendering.

A zero return does not mean nothing happened. A pump with an empty queue may still
report deferred unhandled async faults through the VM error handler before it
returns, which is where a rejected Future that nobody awaited finally surfaces.

`sqasync::has_pending` reports whether the runtime has work it can advance on its
own. It is the shutdown condition for a drain loop, but only in combination with the
host's other signals: a task parked on a Future that nothing will ever settle is
not pending, because it cannot advance without outside input. Draining until
`has_pending` is false, and no further, is what makes shutdown terminate instead of
spinning.

## Threading

The unit of affinity is the VM, not the process. Each VM owns a step queue that only
its own thread touches, and an inbox protected by a mutex that any thread may push
into. Whichever thread calls `pump(v)` drains both. Different VMs may be pumped on
different threads.

That split is why there are two posting primitives:

- `sqasync::post_on_vm_thread` is lock free and must be called from the thread that
  pumps that VM.
- `sqasync::post_from_any_thread` takes the inbox lock and is callable from
  anywhere. This is the one a worker thread uses to hand a finished result back.

Choosing the first from the wrong thread is a data race, not an error return, so it
is worth being sure which thread a callback runs on before picking.

## Settling a Future from C

`sqasync::future_create` makes one and writes an addref'd handle out, which the host
holds while the work is in flight. `future_resolve` and `future_throw` settle it,
and the task that was awaiting resumes on the next pump. The handle is yours to
release once it is settled.

The full list, with the header's own notes, is on
[Async runtime](page:capi/async).
