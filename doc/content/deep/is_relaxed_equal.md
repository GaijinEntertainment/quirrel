---
see_also: [deep.is_strict_equal, deep.copy, math.deep_hash]
---

Compares `a` and `b` deeply, with the same steps, results and errors as the
`isEqual` function of the Dagor script library (`%sqstd/underscore.nut`).
`is_strict_equal` does the same steps, but compares numbers by type and bits.

## Parameters

- `a` - the first value
- `b` - the second value
- `comparers` - an optional table of compare functions, or `null`. A key is a
  type name as `type()` gives it, a class, or a function that tells if it
  applies to an instance

## Return value

The value that a compare function or an `isEqual()` method returns when it
decides for `a` and `b` themselves, else `true` or `false`.

## Errors

An error in an `isEqual()` method, a compare function or a `len()` call goes to
the caller; a class with no `len` member has no `len()`, so two such classes
throw. An instance with no `isEqual()` and a `null` value for `comparers`
throws `cannot iterate null`. A compare that keeps walking through a cycle
throws `stack overflow, cannot resize stack` after 10997 nested pairs, where
the script stops when it is called from the top level.

## Notes

For each pair of values:

1. `a == b` gives `true`: the same object, the same string, or equal numbers,
   so `1` and `1.0` are equal.
2. Different type names give `false`.
3. A compare function under the type name in `comparers` decides.
4. Tables, arrays and classes are equal when `len()` gives the same value and
   each key of `a` is in `b` with an equal value.
5. An instance with an `isEqual` member decides with `a.isEqual(b)`. If it has
   no such member, the first key of `comparers` (in `foreach` order, which is
   not defined) that is a function true for both values, or a class that both
   values are instances of, decides with its function. If no key applies, the
   result is `false`.
6. Other values, such as functions, are equal only by steps 1 and 3; a weakref
   read from a container is compared as its target.

A value read from a frozen container is frozen when it goes to an `isEqual()`
method or a compare function, as `foreach` and `[]` make it.

The `_cmp` metamethod is not used: it orders values, and `==` does not use it
either. A class that wants it writes `function isEqual(o) { return (this <=> o) == 0 }`.

## Example

{{example:deep.is_relaxed_equal}}
