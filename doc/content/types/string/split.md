---
params: [separator]
see_also: [types.String.split_by_chars, types.String.join]
---

Splits `str` into an array of strings.

## Parameters

- `separator` - the literal substring to split on; if omitted, `str` is split
  on runs of white space instead
- `...` - not a real parameter; see Notes

## Return value

With no `separator`, `str` is split on runs of white space (as in
[string.strip](sym:string.strip)): leading and trailing white space produce
no empty pieces, and a run of several white-space bytes counts as one
separator.

With `separator` given, `str` is split wherever that exact substring occurs,
matched as a whole rather than as a set of bytes - contrast
[types.String.split_by_chars](sym:types.String.split_by_chars), which treats
each byte of its argument as its own separator. Every match produces a piece,
including empty ones: a leading or repeated separator makes an empty piece,
and, unlike `split_by_chars`, a trailing separator also makes an empty piece
at the end, because this form always emits whatever follows the last match,
even nothing.

## Errors

Throws `empty separator` when `separator` is `""`.

## Notes

Takes 0 or 1 argument, not the variadic tail the VM's dump implies (it shows
`split([arg1: string], ...)` because this binding carries no declaration
string). A second argument is silently ignored.

## Example

{{example:types.String.split}}
