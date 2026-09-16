---
see_also: [datetime.time]
---

Returns the CPU time the process has used so far.

## Return value

Seconds of CPU time as a `float`, measured from process start.

## Notes

This is CPU time, not wall-clock time: it does not advance while the process
is idle, for example while blocked on I/O or asleep, so it can read well
behind [time](sym:datetime.time). Use `time` to measure how much real time has passed.

## Example

{{example:datetime.clock}}
