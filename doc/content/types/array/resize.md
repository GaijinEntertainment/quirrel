---
params: [size]
see_also: [array, types.Array.clear]
---

Grows or shrinks the array to `size` elements.

## Parameters

- `size` - the new element count

## Return value

This array.

## Errors

Throws `resizing to negative length` when `size` is negative.

## Notes

This binding's type mask covers only `size`, so the VM's signature shows
just that one parameter with a trailing `...`; in fact `resize` also takes a
second, optional fill value for the slots a growing resize adds (`null`
when left out), the same fill value the [array](sym:array) constructor
takes, with the same aliasing caveat: it is stored as is, not cloned per
slot. That fill value is entirely invisible to the VM's introspection, since
the mask never mentions it; this page is the only place it is written down.
A third real argument beyond that is silently ignored.

Shrinking to a quarter or less of the array's reserved capacity releases
the unused storage, the same as [clear](sym:types.Array.clear) does at
`size` `0`.

## Example

{{example:types.Array.resize}}
