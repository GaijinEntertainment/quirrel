---
see_also: [types.Thread.call, types.Thread.wakeup]
---

Returns the thread's current state as a string.

## Return value

One of:

- `"idle"` - never started, or its function already returned or threw
- `"running"` - currently executing (only observable from inside itself, or
  from a nested thread it started)
- `"suspended"` - paused in a `suspend` call, waiting for `wakeup` or
  `wakeupthrow`

## Notes

A thread has no `"dead"` state: once its function returns or throws, it goes
back to `"idle"`, the same state it started in, and `call` on it starts the
function over. Compare `types.Generator.getstatus`, whose generator goes to a
permanent `"dead"` instead.

## Example

{{example:types.Thread.getstatus}}
