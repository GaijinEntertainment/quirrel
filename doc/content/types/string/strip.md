---
see_also: [types.String.lstrip, types.String.rstrip, string.strip]
---

Returns `str` with white space removed from both ends.

## Return value

A new string with the leading and trailing white space removed. See
[string.strip](sym:string.strip) for which bytes count as white
space and what happens to an all-white-space or empty `str`.

## Notes

Takes no arguments. This method and [string.strip](sym:string.strip) call
the same C++ implementation with the same behavior; `"  hi  ".strip()` gives
the same result as `strip("  hi  ")`. They differ only in how the VM's
introspection describes them: the module function is registered with
`pure fastcall` in its declaration string, while this method is registered
through a type-mask table that carries neither attribute, so its badges are
empty even though the underlying computation is also pure.

## Example

{{example:types.String.strip}}
