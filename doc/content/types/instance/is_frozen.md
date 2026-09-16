---
see_also: [types.Instance.rawset, types.Instance.swap, freeze]
---

Reports whether this reference to the instance was frozen with `freeze()`.

## Return value

`true` if the instance is immutable, `false` otherwise.

## Notes

Takes no arguments. The immutable flag lives on the reference, not on the
instance: `freeze(someInstance)` returns a new, frozen reference to the same
instance, but a variable that already held it keeps pointing at an unfrozen
reference. `someInstance.is_frozen()` stays `false` unless the variable itself
is reassigned to the frozen result.

A frozen instance rejects `rawset` and `swap` with `Cannot modify immutable
object`. `clone()` is not blocked by this flag: cloning a frozen instance
gives back a plain, unfrozen instance, since the flag never survives a copy.

## Example

{{example:types.Instance.is_frozen}}
