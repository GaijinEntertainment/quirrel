---
title: Raw object handling
group: C API
order: 132
summary: `HSQOBJECT` handles and reference counting.
---

Working with an `HSQOBJECT` handle instead of a stack slot, and reference counting.

Two different things live here, and the header groups them together because both
step outside the stack.

**Handles.** `sq_getstackobj` takes a handle to a value, and
`sq_addref` / `sq_release` control its lifetime. This is how a host keeps a script
value across frames. See
[Holding references from C](page:embedding/objects) for the sequence, which is easy
to get wrong in a way that only shows up much later.

**Handle-direct access.** The `sq_obj_*` family reads and writes through a handle
without touching the stack at all, which saves the push and pop around a hot read.

{{capi:raw object handling}}
