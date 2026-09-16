---
see_also: [debug.script_watchdog_kick]
---

Sets the script watchdog timeout in milliseconds and returns the previous value.

## Parameters

- `timeout_msec` - milliseconds of wall-clock time a script may run before the
  watchdog raises an error; `0` turns the watchdog off

## Return value

The timeout that was in effect before this call.

## Notes

The watchdog guards against a script that never returns control, such as one
caught in an infinite loop. The VM checks the elapsed time periodically while
running bytecode, not on every single instruction, and raises a catchable
`"Watchdog: too long execution of quirrel script"` error once the timeout is
exceeded. Setting a new timeout also resets the clock, the same as
`script_watchdog_kick` does.

## Example

{{example:debug.set_script_watchdog_timeout_msec}}
