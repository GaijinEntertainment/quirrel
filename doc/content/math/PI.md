---
see_also: [math.sin, math.cos, math.atan2]
---

The ratio of a circle's circumference to its diameter, for converting between
radians and degrees and for angle math in general.

## Notes

Every trig function in `math` takes and returns radians, not degrees, so a
degree value needs `degrees * PI / 180` before it reaches `sin`, `cos`, `tan`,
or their inverses.

The value comes straight from the C `M_PI` macro, cast to `SQFloat`.
