---
title: async and await
group: Language
order: 70
summary: `async` functions, `await`, and the Future an async call returns.
---

`async` and `await` are a cooperative scheduler built on top of
[generators](page:language/generators). An `async` function returns a
[Future](sym:async.Future) to its caller at once, suspends at each `await`, and
resumes when the awaited value is ready. It resumes on the same VM, never on a
separate thread.

## Declaring an async function

Write `async` before any function declaration form to make the function async:

```nut
async function loadStuff() { return await fetchIt() }

let f = async function() { return await pending() }
let direct = async @() "ready"
let echo = async @(msg) await msg

class Squad {
  async function resupply(ammoCount) { return await deliver(ammoCount) }
}
```

`async constructor` is rejected, because a constructor must return the new
instance, not a Future. `async` on a metamethod (`_tostring`, `_add`, ...) is also
rejected, because a metamethod must return its value synchronously.

A call to an async function does not run any part of its body at once, not even
the part before the first `await`. The call returns a new, pending
[Future](sym:async.Future) immediately. The first step of the body runs only when
something drives the scheduler forward; see [Scheduling](#scheduling) below. A
`return` inside the body settles that Future as fulfilled with the returned value.
An uncaught `throw` settles it as faulted.

{{example:language/async-mission-briefing}}

## await

`await <expr>` suspends the enclosing async function until the value of `<expr>` is
ready, then produces that value. It unwraps one level, as
[Future.resolve](sym:async.Future.resolve) describes. A value that is not a
`Future` is returned unchanged at the next scheduler step, so `await` on a plain
value is valid and does nothing.

`await` only compiles inside an async function; writing it in a plain one is a
compile-time error.

A `throw` inside an async function faults its Future with the thrown value. `await`
throws that value again at the await site. An ordinary `try`/`catch` there catches
it like a synchronous throw.

{{example:language/async-fault-propagation}}

A faulted Future that no code awaits or acknowledges produces an unhandled-fault
report. See [Future.reject](sym:async.Future.reject) and
[Future.markHandled](sym:async.Future.markHandled) for that report and how to stop
it. [Future.all](sym:async.Future.all) and [Future.race](sym:async.Future.race)
run several Futures concurrently. `all` fulfills when every input has fulfilled,
and faults as soon as any input faults. `race` settles as soon as the first input
settles, with a fulfil or a fault.

## Scheduling

Code after an async call in the same script frame runs before the body of that
call. This is why the mission-briefing example above prints `continuing setup`
before `loading briefing`. The scheduler is a FIFO queue. A pump call drains the
pending work. When a Future settles, every waiter parked on it resumes in the
order it parked.

## The plain interpreter versus the Dagor host

Core Quirrel's `async` module has one export, the `Future` class (with its static
`all`/`race`). The plain interpreter, `sq`, sets up the runtime by itself. Before
it runs the script, it binds the runtime and registers the module from C++. After
the top-level script has finished, it drains the scheduler in a loop. Both
examples on this page run under the plain interpreter without changes. No host
code is needed for `Future`, `async` or `await` to work.

The plain interpreter cannot create a Future from an external event. The Dagor
game host adds natives for that to the same `async` module table:
`async.delay(seconds)` and `async.nextFrame()`. The game's own timers back them,
and a `pump()` call that the host makes once per frame settles them. `sq` never
calls `pump()` on a frame tick. It calls `pump()` in a bounded loop right after
the script's own code has finished. A script that awaits `async.delay` under the
plain interpreter does not hang; it fails to compile, because `delay` does not
exist there. An HTTP client that resolves a Future from a network response is
also a Dagor-specific binding, not part of core Quirrel.

Because `sq` pumps only once, right after the script ends, a Future that never
settles does not hang the plain interpreter. Its parked continuation is abandoned
when the process exits, with no error and no diagnostic. A chain of Futures that
keeps resolving into more Futures drains inside that same post-script loop, up to
a large iteration cap. The cap is there to catch a runaway chain, not to limit a
normal program.
