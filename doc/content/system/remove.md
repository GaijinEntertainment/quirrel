---
see_also: [system.rename]
---

Deletes the file at `path`.

## Parameters

- `path` - path of the file to delete

## Errors

Throws `remove() failed` if the underlying C `remove()` call fails, for
example because the file does not exist or is in use. The message carries no
further detail: no path, no errno, no reason.

## Notes

Unlike [getenv](sym:system.getenv), [setenv](sym:system.setenv) and [system](sym:system.system), `remove` is
built the same way on every target; there is no platform stub.

No example runs here because deleting a file changes the machine, which every
example on this site must avoid. Typical use:

```nut
from "system" import remove

try {
  remove("scratch.tmp")
} catch (e) {
  println(e) // "remove() failed"
}
```
