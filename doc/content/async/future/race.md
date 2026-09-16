---
see_also: [async.Future.all]
---

Settles as whichever future in `arr` settles first, fulfilled or faulted.

## Parameters

- `arr` - array of futures (or plain values) to wait on

## Return value

A fresh `Future`. The first input to fulfil fulfils it with that value; the
first input to fault faults it with that value - a fault wins the same way
a fulfilment does, whichever happens first. Every losing input is
discarded with no unhandled-fault report, since `race` itself read it.

## Errors

Throws `Future.race: expected an array` when `arr` is not an array,
including when it is omitted, and `Future.race: empty array` for an empty
one - checked on the calling frame, so a plain `try`/`catch` catches both
without `await`. This differs from JS `Promise.race`, where an empty array
returns a future that never settles; here that shape is treated as a bug
and reported at the call site instead.

## Notes

When several inputs are already settled before `race` runs, the earliest
one in array order wins (matching JS `Promise.race` reaction order). A
plain value that is not a `Future` settles immediately - the same
pass-through behavior `await` gives it elsewhere - so it wins over a
still-pending future input even when listed after it in `arr`.

## Example

{{example:async.Future.race}}
