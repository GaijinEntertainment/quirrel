---
title: Async runtime
group: C API
order: 140
summary: The C entry points of the async runtime.
---

The C side of [async and await](page:language/async): installing the runtime,
driving it once per frame, and settling a Future from native code.

The runtime does not run itself. `sqasync::pump` is what advances every task
waiting on a Future, and a host that never calls it has async functions that start
and never finish. See [The async runtime](page:embedding/async) for where to put
that call and how to shut down cleanly.

The descriptions here come from the header, which documents these more fully than a
one-line summary can.

{{capi:async}}
