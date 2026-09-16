---
see_also: [math.hash]
---

Returns a hash of `obj` that also looks at what it holds, down to `depth`
levels of nesting.

## Parameters

- `obj` - the value to hash
- `depth` - how many levels of nesting to hash into; defaults to 200

## Return value

A non-negative `int`. `deep_hash(obj, 1)` always equals `hash(obj)`, since
both then look only at `obj` itself.

## Errors

Throws `hashing depth must be between 1 and 200` when `depth` is less than 1
or greater than 200. The same "unsupported type for hashing" error as `hash`
applies to any value in the tree that has no hash, at whatever depth it is
found.

## Notes

Nesting beyond `depth` is not an error; it is not looked at, the same
way `hash` does not look past its own object. `deep_hash({a = {x = 1}}, 2)`
and `deep_hash({a = {x = 2}}, 2)` are equal, because the inner table's own
keys and values sit one level past what depth 2 reaches; depth 3 is needed to
tell them apart.

For a table, the hash of each key and value is folded in as the VM visits
them, in whatever order that happens to be, so the result depends on that
order. The VM can randomize table iteration order between runs (the test
tooling exposes this as `-iter-seed`), so `deep_hash` of a table is stable
across repeated calls within one run but is not guaranteed to reproduce the
same value in a different run or build. An array has no such gap: elements
are always visited by index, in order.

## Example

{{example:math.deep_hash}}
