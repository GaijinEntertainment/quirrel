---
see_also: [datetime.clock, datetime.date]
---

Returns the current wall-clock time.

## Return value

Seconds since the Unix epoch (1970-01-01 00:00:00 UTC), as an `int`.

## Notes

This reads the system clock, so it follows any change to it: a clock set
backward by the user or by NTP can make one reading lower than an earlier
one. [date](sym:datetime.date) converts a value like this one into calendar fields, and
uses this function itself for its own current-time default.

## Example

{{example:datetime.time}}
