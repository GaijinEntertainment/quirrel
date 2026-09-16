---
see_also: [math.deep_hash]
---

Returns a hash of `obj` itself, without looking inside it.

## Parameters

- `obj` - the value to hash

## Return value

A non-negative `int`. Equal values of a hashable type always give the same
hash; different values are likely, but not guaranteed, to give different
hashes.

## Errors

Throws `unsupported type for hashing` for a value with no hash, such as a
function, generator, thread, weakref, userdata, or userpointer.

## Notes

`hash(obj)` is `deep_hash(obj, 1)`: it mixes in the type and value of `obj`
itself, but a table, array, class, or instance is only one level deep, so its
keys and values are never visited. `hash([1, 2, 3])`, `hash([9, 9])`, and
`hash([])` all return the same value, and so do `hash({a = 1})` and `hash({})`.
Use `deep_hash` when the contents have to matter.

Because contents are never visited, a table's hash does not depend on the
order the VM happens to visit its keys - there is nothing left for that order
to affect. `deep_hash` does not have that immunity; see its Notes.

`hash` and `deep_hash` are the only functions in `math` that are not
`fastcall`. A fastcall native has to be a leaf that pushes a bounded number of
stack values, and hashing a table or array pushes and pops one pair of values
per key through `sq_next`, an amount that depends on the object, so neither
function qualifies.

## Example

{{example:math.hash}}
