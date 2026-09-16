---
see_also: [types.UserData.clone, types.UserData.getfuncinfos, types.Table.is_frozen]
---

Reports whether this userdata was frozen with `freeze()`.

## Return value

`true` if the userdata is immutable, `false` otherwise.

## Errors

Not reachable from plain script: Quirrel gives script code no way to create
a `userdata` value, so there is never one to call this on.

## Notes

Takes no arguments. Registered with a receiver check of `.` (matches any
type), the same as [`types.Table.is_frozen`](sym:types.Table.is_frozen), so
unlike [`getfuncinfos`](sym:types.UserData.getfuncinfos) it does not fall
into the delegate-table lookup trap described there - any `this` passes the
check, whichever type's default method answers it.

```nut
// Illustration only - ud is not obtainable from plain script.
println(ud.is_frozen())
```
