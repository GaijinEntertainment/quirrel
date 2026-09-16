---
see_also: [io.file.constructor, io.file.flush, io.stdout]
---

Closes the file if it is still open. Every other method starts throwing
once this has happened.

## Errors

Calling `close` itself never throws, even a second time on an already
closed file: it is the other methods that react to the closed state, by
throwing `the stream is invalid`.

## Notes

`close` only has an effect on a file this instance owns. The
`userpointer` constructor form can share a handle without owning it (this
is how `io.stdout`, `io.stdin` and `io.stderr` wrap the process's own
streams), and on such an instance `close` does nothing: the handle
stays open and every method keeps working.

## Example

{{example:io.file.close}}
