---
see_also: [system.remove]
---

Renames the file at `old` to `new`.

## Parameters

- `old` - current path of the file
- `new` - new path for the file

## Errors

Throws `rename() failed` if the underlying C `rename()` call fails, for
example because `old` does not exist or `new` is on a filesystem the C
library refuses to move a file to. The message carries no further detail: no
path, no errno, no reason.

## Notes

Unlike [getenv](sym:system.getenv), [setenv](sym:system.setenv) and [system](sym:system.system), `rename` is
built the same way on every target; there is no platform stub.

No example runs here because renaming a file changes the machine, which every
example on this site must avoid. Typical use:

```nut
from "system" import rename

try {
  rename("old.tmp", "new.tmp")
} catch (e) {
  println(e) // "rename() failed"
}
```
