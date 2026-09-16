---
see_also: [string.escape, types.String.subst]
---

Returns `str` with backslash, the two quote characters, and every
non-printable-ASCII byte replaced by an escape sequence.

## Return value

The escaped string. See [string.escape](sym:string.escape) for the exact
escaping rules (which characters get a one-character escape, which get a
`\xNN` hex escape, and why the classic `\t`/`\n` letter forms never appear).
When there is nothing to escape, `str` itself comes back unchanged.

## Notes

Takes no arguments. Same implementation as [string.escape](sym:string.escape);
see the Notes on [types.String.strip](sym:types.String.strip) for how the two
forms differ only in their reported attributes, not their behavior.

## Example

{{example:types.String.escape}}
