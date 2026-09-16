---
see_also: [async.Future.race]
---

Runs every future in `arr` concurrently and fulfils with an array of their
values, in input order, once every one of them fulfils.

## Parameters

- `arr` - array of futures (or plain values) to wait on

## Return value

A fresh `Future`. It fulfils with an array the same length as `arr`, each
slot holding the matching input's settled value in input order - regardless
of the order the inputs settle in. An empty array fulfils right
away with `[]`.

## Errors

Throws `Future.all: expected an array` when `arr` is not an array,
including when it is omitted. This check runs on the calling frame, not
inside a task, so a plain `try`/`catch` catches it without `await`.

Fail-fast: on the first input to fault, the result faults with that
thrown value, and later inputs are ignored - they still run to completion,
but nothing further reads their outcome. The result's fault carries the
bare value the input threw; it does not carry that input's own origin
trace, so a report on an abandoned result attributes the fault to `all`
itself, not to the input that threw.

## Notes

An element of `arr` that is not a `Future` is awaited as-is - the same
pass-through `await` gives any non-future value.

Fail-fast means `all` does not wait for the rest once one input faults, and
it does not aggregate every outcome (matching JS `Promise.all`; unlike .NET
`Task.WhenAll`, which waits for all and aggregates). For "run every input,
collect every outcome whether it faults or not", wrap each input so it
never faults on its own, and call `all` over the wrappers instead of
wrapping the call to `all` itself - a fail-fast `all` only ever hands a
wrapping `try` the first fault.

There is no `Future.any`, by design. Waiting past a fault for some other
input to fulfil would hide a real failure.

## Example

{{example:async.Future.all}}
