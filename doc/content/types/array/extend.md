---
params: [other]
see_also: [types.Array.append, types.Array.replace_with]
---

Appends every element of `other` to this array.

## Parameters

- `other` - an array whose elements are copied in; give more than one to
  chain several in one call

## Return value

This array.

## Errors

Throws `only arrays can be used to extend array` when a second or later
argument is not an array; a non-array first argument is instead rejected by
the VM itself with its own wrong-type message, since that one argument is
covered by the binding's type mask.

## Notes

This binding's type mask covers only the first argument, so the VM's
signature shows one parameter, but `extend` accepts any number of arrays and
appends them in order: `a.extend(b, c)` is `a.extend(b); a.extend(c)` done
in one call. Every argument is checked to be an array before any of them are
appended.

## Example

{{example:types.Array.extend}}
