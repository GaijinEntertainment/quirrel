---
see_also: [io.stdout, io.stdin, io.file.writestring]
---

The process's standard error stream, already open as an
[io.file](sym:io.file) instance.

## Notes

`stderr` is a separate stream from `stdout`; a tool that merges both into
one log (as the reference tests do) only preserves their relative order
because neither stream buffers, so every write lands as soon as it
happens. `stderr` shares the process's handle rather than owning it, so
[close](sym:io.file.close) on it does nothing, the same as on `stdout`.

## Example

{{example:io.stderr}}
