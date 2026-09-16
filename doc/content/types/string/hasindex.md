---
params: [index]
see_also: [types.String.len, types.String.slice]
---

Returns whether `index` is a valid byte position in `str`.

## Parameters

- `index` - the byte index to test
- `...` - not a real parameter; see Notes

## Return value

`true` when `0 <= index < str.len()`, `false` otherwise. Unlike
[types.String.slice](sym:types.String.slice), a negative `index` is not
counted from the end - it is out of range, so it gives `false`.

## Notes

Takes exactly one argument, not the variadic tail the VM's dump implies (it
shows `hasindex(arg1: number, ...)` because this binding carries no
declaration string). A second argument is silently ignored.

## Example

{{example:types.String.hasindex}}
