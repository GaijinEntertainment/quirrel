---
see_also: [async.Future.getState, async.Future.markHandled, async.Future.resolve]
---

Returns the settled value: the fulfilled value, or the fault value.

## Return value

Whatever the future settled with, verbatim - the fault value on a faulted
future, not the fault re-raised as an error.

## Errors

Throws `Future.getValue: future is still pending` while the future has not
settled yet, so `null` can stay a valid settled value: check
[getState](sym:async.Future.getState) first if `null` would otherwise be
ambiguous.

## Notes

This is the synchronous read; `await` is the asynchronous one, and unlike
`getValue`, `await` re-raises a fault instead of returning it.

`getValue` is a pure peek: reading a faulted future's value this way does
NOT acknowledge the fault, so an otherwise-unconsumed fault still reports
as unhandled. Take responsibility for it with
[markHandled](sym:async.Future.markHandled), or consume it with `await`.

## Example

{{example:async.Future.getValue}}
