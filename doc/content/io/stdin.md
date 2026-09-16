---
see_also: [io.stdout, io.stderr, io.file.readn, io.file.readblob]
---

The process's standard input stream, already open as an
[io.file](sym:io.file) instance.

## Notes

`stdin` shares the process's handle rather than owning it, so
[close](sym:io.file.close) on it does nothing. Reading from it blocks
until data (or end-of-file) arrives, which depends on what the process was
launched with, so there is no example here: a doc page that reads `stdin`
would hang or vary from one run to the next, and this site's examples must
not do either. The read side is the same as any other file:

```nut
from "io" import stdin

let line = stdin.readblob(256)  // blocks until input or end-of-file arrives
```
