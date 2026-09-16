---
see_also: [error, println]
---

Writes its arguments to the error stream, followed by a newline.

## Parameters

- `...` - values to write; each is converted the same way `print` converts its arguments

## Return value

`null`.

## Notes

Same stream as `error`, and the same one-space join between several
arguments; the only difference from `error` is the trailing newline, matching
how `println` differs from `print`.

## Example

{{example:errorln}}
