---
title: Lexical structure
group: Language
order: 10
summary: Comments, identifiers, numbers, and where one statement ends.
---

This page covers the tokens the compiler sees before it looks at grammar at
all: comments, identifiers, keywords, number and boolean literals, and where
one statement ends and the next begins. String literals have their own page,
[Strings](page:language/strings).

## Comments

`//` starts a comment that runs to the end of the line. `/* ... */` runs
until the next `*/`, across as many lines as it needs, but it does not
nest - the first `*/` closes the comment even if an inner `/*` came after
the outer one.

{{example:language/lexical-comments}}

## Identifiers and keywords

An identifier starts with a letter or `_`, followed by any number of
letters, digits or `_`. Quirrel is case sensitive, so `squad`, `Squad` and
`SQUAD` are three different names.

These words are reserved and cannot be used as an identifier. Each links to
the section that explains it.

{{keywords}}

Two of these are narrower than they look:

- `not` only appears directly before `in`, as the compound operator `x not
  in t`. There is no standalone `not expr`; write `!expr` for that.
- `clone`, `switch`, `case` and `default` are reserved only while the
  language feature they belong to is turned on, which is the default. A
  [compiler directive](page:language/directives) can turn either off for a
  unit, and once it is off the word parses as an ordinary identifier -
  `#forbid-clone-operator` lets you declare `function clone(x) { ... }`.

`async` and `await` mark and unwrap an asynchronous function; that is a
feature of its own, not covered here.

## Numbers

An integer is decimal (`30`) or hexadecimal (`0x1E`, up to 16 hex digits).
A leading `0` before another digit is a compile error - octal literals are
not supported. A character between single
quotes, such as `'A'`, is an integer literal too: the value is the one byte
between the quotes, or the one byte an escape such as `'\n'` produces;
anything that is not exactly one byte is an error.

A float needs a fractional part, an exponent, or both: `1.5`, `1.5e3`,
`1e-3`. There is no bare leading dot - write `0.5`, not `.5`.

An underscore may sit between digits anywhere in a number, decimal, hex or
float, purely for readability: `123_456`, `0xFF_00`, `1_000.25` are the same
values as without the underscores.

{{example:language/lexical-numbers}}

## true, false, null

`true` and `false` are the two `bool` values. `null` is its own type and
its own value - it is not `0`, `false`, or an empty string; see
[Values and types](page:language/types) for how equality treats it.

## __FILE__ and __LINE__

`__FILE__` and `__LINE__` are keywords, not variables: the compiler replaces
each with a literal at the position where it appears, `__FILE__` with the
path being compiled and `__LINE__` with that line's number. Because the
substitution happens where the token sits in the source, `__LINE__` inside a
function reports where that line is written, not where the function is
later called from.

{{example:language/lexical-fileline}}

## Statement separators

A newline ends a statement, and so does `;`, which additionally lets two
statements share one line. Either works only at a point where the current
statement could already end - a trailing operator, an open `(`, or a
trailing `,` all keep the parser inside the same statement, so it happily
continues onto the next line looking for the rest of the expression.

{{example:language/lexical-statements}}

## # is a directive, not a comment

A line starting with `#` is not a comment - it is a
[compiler directive](page:language/directives), which changes how the rest
of the file compiles rather than being ignored. Do not confuse it with `//`.
