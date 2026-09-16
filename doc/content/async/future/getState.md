---
see_also: [async.Future.getValue, async.Future.resolve, async.Future.reject]
---

Returns the current lifecycle state.

## Return value

One of the strings `"pending"`, `"fulfilled"` or `"faulted"`.

## Notes

State only moves forward, and only once: `pending` to `fulfilled` or
`pending` to `faulted`. Once settled, a future stays in that state for the
rest of its life; a later `resolve` or `reject` call is a silent no-op and
does not change what `getState` reports.

## Example

{{example:async.Future.getState}}
