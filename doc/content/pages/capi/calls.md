---
title: Calls and errors
group: C API
order: 134
summary: Calling a closure, and raising and reading an error.
---

Calling a script function from C, and raising and reading errors.

A call is set up on the stack: push the closure, push its `this`, push the
arguments, then `sq_call` with the count. `sq_call` pops what it was given and
pushes the result if asked for one. See
[Compiling and calling](page:embedding/running) for the sequence.

Errors travel the same way a script `throw` does. A `sq_throwerror` from a
native can be caught by a `try` in the script that called it. `sq_getlasterror`
reads the value back on the C side.

{{capi:calls}}
