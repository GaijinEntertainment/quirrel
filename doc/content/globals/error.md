---
see_also: [errorln, print]
---

Writes its arguments to the error stream.

## Parameters

- `...` - values to write; each is converted the same way `print` converts its arguments

## Return value

`null`.

## Notes

Goes to the error stream (`stderr` on the command-line host), a stream `print`
and `println` never touch. Several arguments are joined with a single space,
same as `print`, and `error` adds no trailing newline of its own, same as
`print` too - call `errorln` for that.

The example on this page prints on both streams. The site's exec-test runner
captures them merged into one file, in the order each write happened
(the host disables output buffering to keep that order stable), so a mix of
`print`-family and `error`-family calls is safe to commit here.

## Example

{{example:error}}
