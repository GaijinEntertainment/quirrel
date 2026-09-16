---
see_also: [math.rand]
---

Sets the seed used by `rand`.

## Parameters

- `seed` - the new seed

## Return value

Nothing; the call returns `null`.

## Errors

Throws `wrong number of parameters passed to native closure 'srand' (N
passed, 2 required)` when called with no argument, and a parameter type
error when `seed` is not an integer (a float is not accepted).

## Notes

`seed` is stored as an unsigned 32-bit value, so a negative seed wraps to
the same value as `seed + 2^32`: `srand(-1)` and `srand(4294967295)` start
the same sequence.

The seed lives in the shared state, one per VM rather than one per module,
so this change is visible to `rand()` calls from every module.

## Example

{{example:math.srand}}
