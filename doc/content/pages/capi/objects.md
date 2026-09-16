---
title: Creating objects
group: C API
order: 128
summary: Pushing C values, creating objects, and reading values back.
---

Pushing C values into the VM, making containers and classes, and reading values
back out.

This is the largest section of the header. It holds three related jobs: the
`sq_push*` family puts a C value on the stack, the `sq_new*` family creates a
Quirrel object, and the `sq_get*` family converts a value on the stack back to
C.

A `sq_get*` returns `SQRESULT` because the conversion can fail. A read of a
string from a slot that holds an integer is an error, not a coercion. A string
pointer obtained this way is owned by the VM. It stays valid only while the
value is reachable from the stack or from a handle you hold a reference to. See
[Holding references from C](page:embedding/objects).

Use `sq_new_closure_slot_from_decl_string` to register a native function, not
`sq_newclosure` plus a type mask. The declaration string gives the function
parameter names and types. Every signature on this site is built from one.

{{capi:object creation handling}}

## Native fields

A class whose instances carry a C++ struct can expose the fields of that struct
directly, without a getter closure per field.

{{capi:native fields}}
