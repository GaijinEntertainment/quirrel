---
see_also: [string.printf, string.escape]
---

Formats `fmt` with the following arguments, following the syntax of the C
`printf` family, and returns the result as a new string.

## Parameters

- `fmt` - the format string
- `...` - one value per conversion in `fmt`

## Return value

The formatted string.

## Errors

Throws `invalid format` for a conversion this engine does not recognize,
including a `*` width or precision (taken from an argument, as C allows) -
that is never supported here.

Throws `not enough parameters for the given format string` when `fmt` needs
more arguments than were given. The check happens one conversion at a time as
`fmt` is scanned, not once for the whole call; an extra, unused argument is
never an error.

Throws `string expected for the specified format` when `%s` gets a
non-string argument. Throws `integer expected for the specified format` for
`%d`, `%i`, `%o`, `%u`, `%x`, `%X` or `%c` when the argument is neither an
integer nor a float (a float argument is truncated, not rejected). Throws
`float expected for the specified format` for `%f`, `%g`, `%G`, `%e` or `%E`
when the argument is neither a float nor an integer.

## Notes

The supported conversions are `s`, `i`, `d`, `o`, `u`, `x`, `X`, `c`, `f`,
`g`, `G`, `e`, `E`, and `%%` for a literal `%`. Flags (`-+ #0`) and up to two
width digits and two precision digits are read the same way `printf` reads
them; a width or precision of three digits or more throws `width format too
long` or `precision format too long` instead of being accepted.

A `%f`/`%g`/`%G`/`%e`/`%E` conversion always prints `.` as the decimal
separator, whatever the C locale of the process says.

## Example

{{example:string.format}}
