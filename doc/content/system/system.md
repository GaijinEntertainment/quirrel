---
see_also: [system.getenv, system.setenv]
---

Runs `cmd` through the platform command shell.

## Parameters

- `cmd` - command line to run

## Return value

The exit status the C `system()` call returns for `cmd`. On PC targets other
than Windows that is the raw wait status the platform's C library reports for
the child process, not necessarily the plain 0-255 exit code.

## Notes

This function only exists in this form on a desktop target. A host built with
`SQ_SYSTEM_STUBS=1` (consoles, mobile) replaces it, [getenv](sym:system.getenv)
and [setenv](sym:system.setenv) with stubs, whose docstring reads "Stub:
system() is not available on this platform". That stub throws `system() not
available for this platform`, so code meant for those targets should treat this
function as absent and catch the error, or avoid it.

No example runs here because running a shell command changes the machine,
which every example on this site must avoid. Typical use:

```nut
from "system" import system

let status = system("echo hello")
```
