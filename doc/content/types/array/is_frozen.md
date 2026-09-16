---
see_also: [freeze, types.Array.clone]
---

Reports whether this array reference is frozen.

## Return value

`true` if this reference is immutable, `false` otherwise.

## Notes

[freeze](sym:freeze) marks a reference immutable, not the underlying array:
it returns a new, frozen alias and leaves the reference it was given alone.
Two variables that alias the same array can disagree about `is_frozen`, and
mutating through the unfrozen one is visible through the frozen one too,
since both still point at one array; only the immutable alias itself refuses
to be the one doing the mutating.

## Example

{{example:types.Array.is_frozen}}
