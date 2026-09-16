---
title: async and await
group: Language
order: 70
summary: `async` functions, `await`, and the Future the caller gets.
---

`async` and `await` are a cooperative scheduler built on top of
[generators](page:language/generators): an `async` function suspends at `await`,
hands a [Future](sym:async.Future) to its caller right away, and resumes later - on the
same VM, never on a separate thread - once the awaited value is ready.

## Declaring an async function

Prefix `async` on any function declaration form to make it async-capable:

```nut
async function loadStuff() { return await fetchIt() }

let f = async function() { return await pending() }
let direct = async @() "ready"
let echo = async @(msg) await msg

class Squad {
  async function resupply(ammoCount) { return await deliver(ammoCount) }
}
```

`async constructor` is rejected - a constructor must return the new instance, not a
Future - and `async` on a metamethod (`_tostring`, `_add`, ...) is rejected too, since
those must return their value synchronously.

Calling an async function never runs its body on the spot, not even the part before
the first `await`: the call returns a fresh, already-pending [Future](sym:async.Future)
immediately, and the body's first step only runs once something drives the scheduler
forward - see [Scheduling](#scheduling) below. A `return` inside the body settles that
Future fulfilled with the returned value; an uncaught `throw` settles it faulted.

{{example:language/async-mission-briefing}}

## await

`await <expr>` suspends the enclosing async function until `<expr>`'s value is ready,
then yields that value - one level peeled, as
[Future.resolve](sym:async.Future.resolve) describes. A value that is not a
`Future` is delivered back as-is at the next scheduler step, which is what makes
`await` on a plain value a valid, if pointless, no-op.

`await` only compiles inside an async function; writing it in a plain one is a
compile-time error.

A `throw` inside an async function faults its Future with the thrown value, and
`await` re-raises that value at the await site, where an ordinary `try`/`catch`
catches it exactly like a synchronous throw.

{{example:language/async-fault-propagation}}

A Future nobody ever awaits or acknowledges is not silently forgotten if it faults: see
[Future.reject](sym:async.Future.reject) and
[Future.markHandled](sym:async.Future.markHandled) for the unhandled-fault report and
how to stop it. [Future.all](sym:async.Future.all) and
[Future.race](sym:async.Future.race) run several Futures concurrently:
`all` fulfills once every input has, but faults as soon as any one does;
`race` settles as soon as the first input settles, fulfil or fault.

## Scheduling

Code written after an async call in the same script frame runs before that call's own
body does - the mission-briefing example above prints `continuing setup` before
`loading briefing` for this reason. The scheduler is a FIFO queue: pending work is
drained by pumping it, and every waiter parked on the same Future resumes in the
order it parked, once that Future settles.

## The plain interpreter versus the Dagor host

Core Quirrel's `async` module has exactly one export, the `Future` class (plus its
static `all`/`race`). The plain interpreter, `sq`, wires up the whole runtime by
itself - it calls the C++ equivalent of binding the runtime and registering the
module before running the script, then drains the scheduler in a loop once the
top-level script has finished running. Both examples on this page run under the
plain interpreter unmodified because of this: no host code is needed for `Future`,
`async` or `await` to work.

What the plain interpreter cannot do is manufacture a Future from a real external
event. The Dagor game host adds such natives to the same `async` module table:
`async.delay(seconds)` and `async.nextFrame()`, each backed by the game's own timers
and settled by a `pump()` call the host makes once per real frame. `sq`
never calls `pump()` on a frame tick - it calls it in a bounded loop right after the
script's own code has already finished - so a script that awaits `async.delay` under
the plain interpreter does not hang: it fails to compile, because `delay` does not
exist there at all. Similarly, an HTTP client that resolves a Future from a real
network response is a Dagor-specific binding, not part of core Quirrel.

A consequence of pumping only once, right after the script ends: a Future that never
settles does not hang the plain interpreter either. Its parked continuation is simply
abandoned when the process exits, with no error and no diagnostic - there is nothing
left to drive it forward, and nothing reports that. A chain of Futures that keeps
resolving into more Futures, on the other hand, drains inside that same post-script
loop, up to a large iteration cap meant to catch a runaway chain rather than a normal
program.
