---
see_also: [io.file.close, io.stdout, system.remove]
---

Opens the file at `path` with `mode` and returns it. A second form wraps an
already-open C `FILE*` given as a `userpointer`, which script code has no way
to obtain; that form exists for the host embedding the VM, not for scripts
(it is how [io.stdout](sym:io.stdout), `io.stdin` and `io.stderr` are made).

## Parameters

- `path` - the file to open, or the `userpointer` handle to wrap
- `mode` - an `fopen` mode string for the first form; for the second form,
  ownership of the handle (non-null closes it on destruction, `null` shares
  it)

## Errors

Throws `invalid file mode` when `mode` is not one of the accepted strings,
checked before `path` is touched. Throws `cannot open file` when the
mode is fine but the underlying `fopen` fails, for example because `path`
does not exist or its directory does not.

## Notes

`mode` must be `r`, `w` or `a`, optionally followed by `+`, `b` or `t` in
either order (`r+`, `rb`, `r+b`, `rb+`, `rt+`, and so on) - the same set
`fopen` accepts. `b` and `t` only matter on a platform where the C library
translates line endings in text mode: opening with `w` (no `b`) and writing
`"\n"` can land 2 bytes on disk, not 1, while `wb` writes exactly what was
asked. Prefer a `b` mode whenever the byte count has to be exact, which
`writestring`, `writen` and `writeblob` all promise.

A file does not have to be closed by hand for its data to survive: the
instance is reference-counted, so dropping the last reference destroys it
immediately, which flushes and closes the handle the same as calling
[close](sym:io.file.close). Only a reference cycle defers that to the next
run of [collectgarbage](sym:debug.collectgarbage).

## Example

{{example:io.file.constructor}}
