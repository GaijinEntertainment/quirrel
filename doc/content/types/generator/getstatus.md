---
see_also: [types.Generator.weakref, types.Thread.getstatus]
---

Returns the generator's current state as a string.

## Return value

One of:

- `"suspended"` - paused at a `yield`, or not yet resumed for the first time
- `"running"` - currently executing (only observable from inside itself)
- `"dead"` - returned, or stopped because of an unhandled exception

## Notes

A freshly created generator - before its first `resume` - already reports
`"suspended"`, the same as one paused mid-body; there is no separate
"not started yet" state.

`"dead"` is permanent: `resume` on a dead generator throws `resuming dead
generator` rather than restarting the body. Compare `types.Thread.getstatus`,
whose thread goes back to `"idle"` instead and can be started again with
`call`.

## Example

{{example:types.Generator.getstatus}}
