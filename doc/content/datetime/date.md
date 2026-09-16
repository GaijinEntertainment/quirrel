---
see_also: [datetime.time]
---

Breaks a Unix time down into calendar and clock fields.

## Parameters

- `time` - seconds since the epoch to convert; the current time from [time](sym:datetime.time) when omitted
- `format` - `'u'` selects UTC, anything else selects the local time zone; `'l'` when omitted

## Return value

A table with the integer fields `sec, min, hour, day, month, year, wday,
yday`. `month` is 0 for January through 11 for December, `wday` is 0 for
Sunday through 6 for Saturday, and `yday` is 0 for the first day of the year:
all three follow the C `tm` struct, not the 1-based numbering people usually
read and write dates with. `year` is the full year, such as `2026`, already
adjusted from the C struct's offset from 1900.

## Errors

Throws `crt api failure` if the underlying `localtime`/`gmtime` call fails,
for example a `time` value the platform's calendar cannot represent.

## Notes

`format` is read loosely: only `'u'` selects UTC, and every other value,
including one that is not `'l'`, falls back to local time. There is no
validation and no error for an unrecognized format.

Local time depends on the machine's time zone, so only a call with an
explicit `time` and `format: 'u'` gives the same table everywhere; that is
what the example below does.

## Example

{{example:datetime.date}}
