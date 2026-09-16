---
see_also: [types.Class.tostring, types.Instance.getmetamethod]
---

Converts the instance to a string.

## Return value

The result of the instance's class's `_tostring` metamethod, if it defines
one. Otherwise the VM's generic default, `"(instance : 0x" + address + ")"`.

## Notes

Unlike a class, an instance does consult a metamethod here: this is the only
built-in type-method where a class-level definition (`_tostring`) reaches the
instance's own conversion, since instance indexing goes through the class's
delegate chain while a class's own indexing does not. See
`types.Class.tostring` for the class object's own, unconditional default.

Since the fallback address varies from run to run, examples on this site can
only check a fixed prefix of the result, not the full string, unless the
class defines `_tostring`.

## Example

{{example:types.Instance.tostring}}
