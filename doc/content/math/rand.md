---
see_also: [math.srand, math.RAND_MAX]
---

Returns a random integer.

## Return value

An integer in the closed range `0` to `math.RAND_MAX`.

## Notes

The generator is a linear congruential one over a 32-bit seed held in the
shared state. Its arithmetic wraps at 32 bits, so the same seed
gives the same sequence of results on every platform and every build.

The seed starts randomized when the process starts, so a `rand()` call with
no prior `srand` call is not reproducible. Call `srand` with a fixed seed
first when a script needs the same numbers on every run.

The seed lives in the shared state, one per VM rather than one per module,
so a seed set from any module changes what every module's `rand()` returns
next.

## Example

{{example:math.rand}}
