---
title: Strings
group: Language
order: 50
summary: Immutable bytes, and the ordinary, verbatim and interpolated forms.
---

A Quirrel string is an immutable sequence of bytes, not of characters, and
[len](sym:types.String.len) counts those bytes. There are three ways to
write one: ordinary `"..."`, verbatim `@"..."`, and interpolated `$"..."`.

## Ordinary strings

`"..."` recognises the usual backslash escapes: `\n \t \a \b \r \v \f`, a
literal `\\`, `\"` and `\'`, and `\0` for a zero byte. `\xHH` writes one byte
from one or two hex digits; `\uHHHH` and `\UHHHHHHHH` write a Unicode code
point encoded as UTF-8, from up to four and up to eight hex digits. Any
other character after a backslash, including a bare `{` or `}`, is a
compile error - escaping those two is only meaningful in an interpolated
string, below.

A newline may not appear directly inside `"..."`; break the string with
`\n` or switch to a verbatim string.

{{example:language/strings-escapes}}

## Verbatim strings

`@"..."` takes every character between the quotes literally - no backslash
escape is processed, and a real newline is allowed, which makes it the
convenient form for a multi-line block of text. Because `\` is not special,
the only way to put a `"` inside one is to write it twice. The newline it
keeps is the one in the file, so a file saved with CRLF puts a `
` in the
string as well.

{{example:language/strings-verbatim}}

## Interpolated strings

`$"..."` mixes text with `{expr}` holes; each hole is evaluated, converted
to its string form, and spliced into the result. Under the hood the whole
literal becomes a call to [subst](sym:types.String.subst) with one `{n}`
placeholder per hole, so `$"{a}: {b}"` compiles to essentially
`"{0}: {1}".subst(a, b)`.

A literal brace in the text has to be escaped as `\{` or `\}`, since a bare
`{` opens a hole. A hole's expression can be anything, including another
`$"..."` - the inner literal is parsed as its own independent expression, so
one interpolated string may be nested inside another's hole.

{{example:language/strings-interpolation}}

## Building strings

[concat](sym:types.String.concat), [join](sym:types.String.join) and
[subst](sym:types.String.subst) are type methods, callable on any string
(commonly `""` or a separator string), and are the recommended way to build
one from parts.

`+` also concatenates once either operand is a string, but avoid it: it is
still plain left-to-right `+`, so a string appearing in the middle of a
chain of numbers changes what the earlier `+`s do, not only the later ones.
The static analyzer flags this as `w264`. See
[Operators and expressions](page:language/operators) for how `+` decides
between adding and stringifying.

{{example:language/strings-concat}}

## Bytes, not characters

Because a string is bytes, a multi-byte UTF-8 character counts as more than
one toward `len`, `slice` or an index - there is no separate "character
count". Build a non-ASCII byte sequence with `\x` or `\u` escapes when a
script needs to be portable across source encodings.

{{example:language/strings-bytes}}
