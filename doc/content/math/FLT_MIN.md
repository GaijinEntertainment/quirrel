---
see_also: [math.FLT_MAX]
---

The smallest positive value `SQFloat` can represent at full precision.

## Notes

Taken directly from the C `FLT_MIN` macro in `<float.h>`, cast to `SQFloat`.
Despite the name, this is not the most negative float: it is the smallest
number above zero a normalized float can hold. The true minimum, the most
negative finite value, is `-FLT_MAX`.
