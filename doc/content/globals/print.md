---
see_also: [println, error]
---

Writes its arguments to the output stream.

## Parameters

- `...` - values to write; each is converted the way `tostring` would convert it

## Return value

`null`.

## Notes

Several arguments in one call are joined with a single space; two separate
calls are not - nothing is inserted between them, so `print("a"); print("b")`
reads `ab`. `print` adds no trailing newline; call `println` for that, or end
with a bare `println()`.

Goes to the output stream (`stdout` on the command-line host) - the stream
`error` and `errorln` never touch.

## Example

{{example:print}}
