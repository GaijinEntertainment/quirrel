---
title: Strings
group: Language
order: 50
summary: Ordinary, verbatim and interpolated string literals; strings are immutable bytes.
---

A Quirrel string is an immutable sequence of bytes, not of characters.
[len](sym:types.String.len) counts bytes. There are three literal forms:
ordinary `"..."`, verbatim `@"..."`, and interpolated `$"..."`.

## Ordinary strings

`"..."` accepts the usual backslash escapes: `\n \t \a \b \r \v \f`, a
literal `\\`, `\"` and `\'`, and `\0` for a zero byte. `\xHH` writes one byte
from one or two hex digits. `\uHHHH` and `\UHHHHHHHH` write a Unicode code
point encoded as UTF-8, from up to four and up to eight hex digits. Any
other character after a backslash, including `{` or `}`, is a compile
error. `\{` and `\}` are valid only in an interpolated string (see below).

A newline may not appear directly inside `"..."`. Write `\n`, or use a
verbatim string.

{{example:language/strings-escapes}}

## Verbatim strings

`@"..."` takes every character between the quotes literally. No backslash
escape is processed, and a real newline is allowed, so it is the convenient
form for a multi-line block of text. Because `\` is not special, the only
way to put a `"` inside is to write `""`. The newline in the string is the
newline in the file, so a file saved with CRLF puts a `\r` in the string as
well.

{{example:language/strings-verbatim}}

## Interpolated strings

`$"..."` mixes text with `{expr}` holes. Each hole is evaluated, converted
to a string, and inserted into the result. The compiler turns the whole
literal into a call to [subst](sym:types.String.subst) with one `{n}`
placeholder per hole: `$"{a}: {b}"` compiles to `"{0}: {1}".subst(a, b)`.

A literal brace in the text must be written `\{` or `\}`, because a bare
`{` opens a hole. A hole can hold any expression, including another
`$"..."`. The inner literal is parsed as its own expression, so one
interpolated string can be nested inside a hole of another.

{{example:language/strings-interpolation}}

## Building strings

[concat](sym:types.String.concat), [join](sym:types.String.join) and
[subst](sym:types.String.subst) are type methods. They can be called on any
string (usually `""` or a separator string). They are the recommended way
to build a string from parts.

`+` also concatenates when either operand is a string. Avoid it. `+` is
evaluated left to right, so in `1 + 2 + "a" + 3` the numbers before the
string are added and everything after it is concatenated: the result is
`"3a3"`. The static analyzer flags this as `w264`. See
[Operators and expressions](page:language/operators) for how `+` decides
between adding and stringifying.

{{example:language/strings-concat}}

## Bytes, not characters

Because a string is a sequence of bytes, a multi-byte UTF-8 character counts
as more than one unit for `len`, `slice` or an index. There is no separate
character count. Write a non-ASCII byte sequence with `\x` or `\u` escapes
when a script must be portable across source encodings.

{{example:language/strings-bytes}}
