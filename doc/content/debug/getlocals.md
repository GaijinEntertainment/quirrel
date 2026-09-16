---
see_also: [debug.getstackinfos]
---

Returns a table of local variables at the given stack level.

## Parameters

- `level` - how many frames up from this call to read; defaults to `1`, the
  caller of `getlocals`
- `include_internal` - when true, also include `this`, `vargv`, and any name
  starting with `@`; defaults to `false`

## Return value

A table of local variables, keyed by name, holding whatever has been declared
at `level` up to the point of this call. A `level` this VM has no frame for
gives an empty table, not an error.

## Notes

The default `level` is `1`, not `0`: level `0` would be this call to
`getlocals` itself, and a native function has no script locals of its own to
report.

## Example

{{example:debug.getlocals}}
