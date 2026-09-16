---
see_also: [getroottable]
---

Returns the const table.

## Return value

The const table: a table holding engine-defined constants, such as
`SQOBJ_FLAG_IMMUTABLE` (the flag `getobjflags` reports and `freeze` sets).

## Notes

An ordinary, writable table, not itself frozen; nothing stops a script from
adding its own slots. A script's own `const` and `enum` declarations do not go
through it - those are baked into the compiled code directly and never touch
this table.

## Example

{{example:getconsttable}}
