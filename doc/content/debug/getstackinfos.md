---
see_also: [debug.getlocals, debug.format_call_stack_string]
---

Returns call stack information for the given stack level.

## Parameters

- `level` - how many frames up from this call to look

## Return value

A table with these keys, or `null` when `level` is out of range:

- `func` - the name of the function at that level
- `src` - its source file
- `line` - the line currently executing in it
- `locals` - a table of its local variables, keyed by name

Level `0` is this call to `getstackinfos` itself; `1` is whatever called it,
`2` is that function's own caller, and so on.

## Notes

`locals` includes every name the VM tracks for that frame, including internal
ones such as `this` and `vargv`; it is not filtered the way `getlocals` filters
by default.

## Example

{{example:debug.getstackinfos}}
