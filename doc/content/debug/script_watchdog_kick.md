---
see_also: [debug.set_script_watchdog_timeout_msec]
---

Resets the script watchdog timer.

## Notes

Call this before or after a long but legitimate native operation, so the
watchdog does not mistake it for a runaway script. It has no effect when no
timeout is set.

It returns nothing and never raises, and the clock it resets is not itself
readable from script, so there is no script-visible difference an example
could show. `set_script_watchdog_timeout_msec` returns a value that an
example can print.
