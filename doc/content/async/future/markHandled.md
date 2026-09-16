---
see_also: [async.Future.getValue, async.Future.reject, async.Future.getState]
---

Acknowledges a faulted future so it is not reported as unhandled.

## Notes

Does not consume the value: [getValue](sym:async.Future.getValue) still
reads it afterward, and `getState` still reports `"faulted"`. This is the
explicit counterpart to `getValue`'s peek, for code that inspects a fault
and takes responsibility for it without ever calling `await` on the
future.

A no-op on a future that is already fulfilled. Calling it before the
future settles pre-acknowledges: whatever fault lands on it later is still
not reported. Calling it more than once is harmless.

## Example

{{example:async.Future.markHandled}}
