---
see_also: [deep.is_relaxed_equal, deep.copy]
---

Compares `a` and `b` deeply, with the steps of `is_relaxed_equal`, but two
numbers are equal only when they have the same type and the same bits.

## Parameters

- `a` - the first value
- `b` - the second value
- `comparers` - an optional table of compare functions, or `null`, as for
  `is_relaxed_equal`

## Return value

The value that a compare function or an `isEqual()` method returns when it
decides for `a` and `b` themselves, else `true` or `false`.

## Errors

The same as for `is_relaxed_equal`.

## Notes

Step 1 of `is_relaxed_equal` is `a == b`. This function compares the type and
the bits instead, so `1` and `1.0` are not equal, `0.0` and `-0.0` are not
equal, and a NaN is equal to a NaN with the same bits. Other values are equal as with `==`:
the same object or the same string. Compare functions, `len()` calls and
`isEqual()` methods are called as for `is_relaxed_equal`, so a `float` key of
`comparers` still decides for two floats that are not the same.

## Example

{{example:deep.is_strict_equal}}
