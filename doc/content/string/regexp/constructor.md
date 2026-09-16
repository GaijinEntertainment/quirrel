---
see_also: [string.regexp.match, string.regexp.search, string.regexp.capture]
---

Compiles `pattern` into a reusable `regexp` instance.

## Parameters

- `pattern` - the pattern to compile, in this engine's own syntax (see Notes)

## Return value

A compiled pattern. Call `match`, `search` or `capture` on it any number of
times; the same instance can be reused for every call.

## Errors

Throws a description of the problem when `pattern` does not parse, for
example `unterminated character class`, `expected paren` or `quantifier
value too large`. Throws `backreferences are not supported` for `\1`-`\9`,
and `lazy quantifiers are not supported` for `*?`, `+?` or `??`: both are
syntax this engine recognizes and rejects by design, not syntax it fails
to parse. An excessively large or deeply nested pattern throws `pattern too
complex` or `pattern exceeds maximum allowed nesting depth` while compiling,
before anything is matched.

## Notes

This is not PCRE; it is a small classic-regex engine. What it supports:

- `.` - any byte, including a newline
- `^`, `$` - the start and end of the whole subject string; there is no per-line or multiline mode
- `|` - alternation
- `(...)` - a capturing group; `(?:...)` - a group that does not capture
- `[...]`, `[^...]` - a character class and its negation, with `a-z` ranges
- `*`, `+`, `?`, `{n}`, `{n,}`, `{n,m}` - greedy quantifiers; there is no lazy form, see Errors
- `\d` `\D` `\w` `\W` `\s` `\S` `\a` `\A` `\x` `\X` `\c` `\C` `\p` `\P` - class shorthands for digit, word character, space, letter, hex digit, control and punctuation, and their negations
- `\l`, `\u` - a single lowercase or uppercase letter. Despite the letter used, these match one character; they do not change the case of anything
- `\b`, `\B` - a word boundary, and its negation
- `\m<open><close>`, for example `\m()` or `\m<>` - text balanced between a pair of characters, skipping nested pairs; this is a Quirrel extension with no equivalent in the old table for this engine
- `\t`, `\n`, `\r`, `\f`, `\v`, `\\` - the usual literal escapes

`\1`-`\9` and `\l`/`\u` mean something different, or nothing special,
inside a `[...]` class: there `\1` is just the digit `1` and `\b` is just the
letter `b`, since backreferences and word boundaries make no sense inside a
class.

## Example

{{example:string.regexp.constructor}}
