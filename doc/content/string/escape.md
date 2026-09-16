---
see_also: [string.format]
---

Returns `str` with backslash, the two quote characters, and every byte that
is not printable ASCII replaced by an escape sequence.

## Parameters

- `str` - the string to escape

## Return value

The escaped string. When there is nothing to escape - including when `str`
is empty - `str` itself comes back unchanged.

## Notes

Only three characters get a one-character escape: `\` becomes `\\`, `"`
becomes `\"`, and `'` becomes `\'`. A NUL byte becomes the two characters
`\0`.

Every other byte outside the printable range 32-126 - a tab, a newline, any
other control character, a DEL, or a non-ASCII byte - becomes a four-character
`\xNN` hex escape. It does not become the classic C letter escape (`\t`,
`\n`, `\v`, `\f`, `\r`); older documentation for this function describes
those letter forms, but the code path that would produce them never runs,
because none of those characters is printable ASCII.

## Example

{{example:string.escape}}
