---
see_also: [types.UserData.clone, types.Table.is_frozen]
---

Reports whether this userdata was frozen with `freeze()`.

## Return value

`true` if the userdata is immutable, `false` otherwise.

## Errors

Not reachable from plain script: Quirrel gives script code no way to create
a `userdata` value, so there is never one to call this on.

## Notes

Takes no arguments. Registered with a receiver check of `.` (matches any
type), the same as [`types.Table.is_frozen`](sym:types.Table.is_frozen).

```nut
// Illustration only - ud is not obtainable from plain script.
println(ud.is_frozen())
```
