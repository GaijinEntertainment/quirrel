Returns a table describing the Quirrel build.

## Return value

A table with these keys:

- `version` - the Quirrel version string
- `charsize` - `sizeof(char)` in the build, in bytes
- `intsize` - `sizeof(SQInteger)` in the build, in bytes
- `floatsize` - `sizeof(SQFloat)` in the build, in bytes
- `gc` - `"enabled"` or `"disabled"`

## Notes

These values describe the native build, not the script talking to it, so they
are the same for every VM this process creates. This engine always compiles with
`_SQ64` and without `SQUSEDOUBLE` (see `include/sqconfig.h`), so `intsize` is 8
and `floatsize` is 4 here; do not assume that elsewhere, since both are build
choices upstream Quirrel leaves open.

## Example

The values are build properties and would not stay the same across builds, so
the example below only checks the shape of the table, never a value from it.

{{example:debug.getbuildinfo}}
