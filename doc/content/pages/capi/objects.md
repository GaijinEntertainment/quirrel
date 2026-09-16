---
title: Creating objects
group: C API
order: 128
summary: Pushing C values in, and reading values back out.
---

Pushing C values into the VM, making containers and classes, and reading values
back out.

This is the largest section of the header, and it holds three related jobs: the
`sq_push*` family that puts a C value on the stack, the `sq_new*` family that
creates a Quirrel object, and the `sq_get*` family that converts a value on the
stack back to C.

A `sq_get*` returns `SQRESULT` because the conversion can fail: reading a string
out of a slot that holds an integer is an error, not a coercion. A string pointer
obtained this way is owned by the VM and stays valid only while the value is
reachable from the stack or from a handle you hold a reference to; see
[Holding references from C](page:embedding/objects).

`sq_new_closure_slot_from_decl_string` is the one to reach for when registering a
native function, rather than `sq_newclosure` plus a type mask: the declaration
string is what gives the function real parameter names and types, and it is what
every signature on this site is built from.

{{capi:object creation handling}}

## Native fields

A class whose instances carry a C++ struct can expose that struct's fields directly,
without a getter closure per field.

{{capi:native fields}}
