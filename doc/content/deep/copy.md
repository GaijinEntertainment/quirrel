---
see_also: [deep.copy_graph, deep.is_relaxed_equal, deep.is_strict_equal]
---

Copies `value` deeply, as the `deep_clone` function of the Dagor script library
(`%sqstd/underscore.nut`) does: it copies tables and arrays, gives a class and a
table with a `map` slot to `map()`, and keeps other values. `copy_graph` copies
the object graph instead. The name is `copy` because `clone` is a keyword.

## Parameters

- `value` - the value to copy

## Return value

The copy; an instance comes back as it is.

## Errors

A class and a table with a `map` slot go through `map()`, as in the script, so
a class with no `map` member throws
`the index 'map' (type='string') does not exist`. As in the script, from the
top level of a script 49 nested tables or arrays copy, a 50th only when it is
empty, and deeper ones throw `Native stack overflow`; so do about 50 nested
`map` slots that call the copy function, which the script can copy.

## Notes

Each table and array is copied where it is found. A subtree that two places
share becomes two copies, and a cycle stops at the depth limit. The copies are
not frozen. A weakref read from a container is read as its target, as `foreach`
does, and the target is copied.

## Example

{{example:deep.copy}}
