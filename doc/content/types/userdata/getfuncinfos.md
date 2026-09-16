---
see_also: [types.UserData.is_frozen, types.Table.getfuncinfos, types.UserData.clone]
---

If the userdata has a delegate providing a `_call` metamethod, returns
introspection info about that metamethod's closure; otherwise returns `null`.

## Return value

`null`, or a table describing the `_call` closure - the same shape
[`types.Table.getfuncinfos`](sym:types.Table.getfuncinfos) returns.

## Errors

Not reachable from plain script: Quirrel gives script code no way to create
a `userdata` value, so there is never one to call this on. A C++ host that
creates its own userdata can still reach an error here. This method
is looked up like any other field, and field lookup on a `userdata` with a
custom delegate first tries `Get()` on *that delegate table itself*. If the
delegate table has no `getfuncinfos` slot of its own, that inner `Get()`
does not stop there - a table's own default methods include a `getfuncinfos`
too, bound with a receiver check for `table`, not `userdata`. The lookup on
the userdata ends up resolving to *that* closure, called with the userdata
as `this`, which fails its own receiver check:
`parameter 0 of 'getfuncinfos' has an invalid type 'userdata' ; expected:
'table'`. Methods whose receiver check is `.` (matches anything), such as
`is_frozen` or `clone`, do not show this, since any `this` passes; a
custom-typed one like `getfuncinfos` does.

## Notes

Takes no arguments.

```nut
// Illustration only - ud is a userdata whose delegate is a plain table
// with no getfuncinfos slot of its own (a common shape from the C API).
// This throws the receiver-type error described above, not "null".
println(ud.getfuncinfos())
```
