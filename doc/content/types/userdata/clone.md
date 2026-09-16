---
see_also: [types.UserData.is_frozen, types.UserData.getfuncinfos, types.Table.clone]
---

Always throws.

## Errors

Throws `cloning a userdata` unconditionally. The method exists only so the
`userdata` delegate has a `clone` slot like every other type; the VM's clone
implementation refuses `userdata` (and `class`) outright rather than copying
their payload, since it has no generic way to duplicate whatever a host bound
into the raw bytes.

## Notes

Not reachable from plain script: Quirrel gives script code no way to
create a `userdata` value, so there is never one to call `.clone()` on. A C++
host that creates its own userdata with `sq_newuserdata` hits the same error.

`clone` is also a keyword, so by default the call needs `ud["clone"]()` or
`clone ud`, not `ud.clone()` or `ud.$clone()`. With `#forbid-clone-operator`
the word is an ordinary identifier and `ud.$clone()` compiles. See
[`types.Integer.clone`](sym:types.Integer.clone).

```nut
// Illustration only - `ud` is not obtainable from plain script, and this
// throws "cloning a userdata" regardless.
let copy = clone ud
```
