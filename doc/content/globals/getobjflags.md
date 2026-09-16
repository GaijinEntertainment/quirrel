---
see_also: [freeze, getconsttable]
---

Returns the engine flag bits stored on `obj`'s reference.

## Parameters

- `obj` - any value

## Return value

An `int` bit mask. The only bit currently defined is
`getconsttable().SQOBJ_FLAG_IMMUTABLE`, set by `freeze`; a value that was
never frozen reports `0`.

## Notes

The flags belong to the reference passed in, not to the underlying table or
array: after `let f = freeze(t)`, `getobjflags(f)` is nonzero but
`getobjflags(t)` still reports `0`, because `t` names the object through its
own, never-frozen reference. See `freeze` for what that split means for
writes.

## Example

{{example:getobjflags}}
