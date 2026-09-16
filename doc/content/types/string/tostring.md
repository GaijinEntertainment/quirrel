---
see_also: [types.String.constructor, types.String.tointeger]
---

Returns `str` itself.

## Return value

`str` unchanged: a string converted to a string is the same string.

## Notes

Takes no arguments. The declaration mask for the hidden receiver is `.`
(any type), so the VM's signature dump omits a receiver entirely and shows
this as a bare `tostring(): any` with no `string.` prefix - the same
binding backs `"x".tostring()`, `[1].tostring()` and every other type's
`.tostring()`, they all point at the one shared native function.

## Example

{{example:types.String.tostring}}
