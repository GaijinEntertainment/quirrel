---
see_also: [system.getenv]
---

Sets the environment variable `name` to `value` for the current process.

## Parameters

- `name` - name of the environment variable to set
- `value` - value to give it

## Errors

Throws `setenv() failed` if the underlying call fails (`_putenv_s` on Windows,
`setenv` elsewhere on PC).

## Notes

This function only exists in this form on a PC target. `sqstdsystem.cpp`
builds it, [getenv](sym:system.getenv) and [system](sym:system.system) only `#if _TARGET_PC`; every
other target (consoles, mobile) gets a stub instead, whose docstring reads
"Stub: setenv() is not available on this platform". That stub throws
`setenv() not available for this platform`, so code meant for those targets
should treat this function as absent and catch the error, or avoid it.

No example runs here because setting an environment variable changes the
process, which every example on this site must avoid. Typical use:

```nut
from "system" import setenv, getenv

setenv("MY_VAR", "1")
assert(getenv("MY_VAR") == "1")
```
