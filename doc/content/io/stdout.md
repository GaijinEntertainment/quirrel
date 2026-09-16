---
see_also: [io.stderr, io.stdin, io.file.writestring, io.file.close]
---

The process's standard output stream, already open as an
[io.file](sym:io.file) instance.

## Notes

This is the same underlying stream that `print` and `println` write to, so
output through `stdout` and through `print` interleaves in the order the
calls happen, not in two separate streams.

`stdout` shares the process's handle rather than owning it, so
[close](sym:io.file.close) on it does nothing: the stream stays open and
every method keeps working afterward, unlike a file opened by
[io.file](sym:io.file.constructor) itself.

## Example

{{example:io.stdout}}
