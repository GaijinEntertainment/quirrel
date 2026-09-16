---
see_also: [math.rand]
---

The largest value `rand()` can return; `rand()` results span `0..RAND_MAX`.

## Notes

The value is `0x7FFFFFFF` (2^31 - 1), a fixed constant in the C++ source
(`SQ_MAX_INT_RANDOM` in `sqstdmath.cpp`), not the platform's C `RAND_MAX`. It
stays the same on every build, so code that scales a random value by
`RAND_MAX` behaves the same everywhere.
