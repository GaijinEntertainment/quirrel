---
see_also: [print, errorln]
---

Writes its arguments to the output stream, followed by a newline.

## Parameters

- `...` - values to write; each is converted the way `tostring` would convert it

## Return value

`null`.

## Notes

Same stream as `print`, and the same one-space join between several
arguments; the only difference from `print` is the trailing newline.
`println()` with no arguments writes just that newline.

## Example

{{example:println}}
