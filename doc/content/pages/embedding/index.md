---
title: Embedding Quirrel
group: Embedding
order: 100
group_index: true
---

Quirrel is an extension language. The compiler and the VM are a C library, and a
host application drives them. The API is declared in `squirrel.h`, and the
standard modules in the `sqstd*.h` headers next to it.

This section is the guide. The function-by-function list is in the
[C API reference](page:capi/index).

## The shape of a host

A minimal host does four things, in this order:

```cpp
HSQUIRRELVM v = sq_open(1024);   // a VM with room for 1024 stack slots
sqstd_seterrorhandlers(v);       // so an error is reported rather than swallowed

sq_pushroottable(v);
sqstd_register_mathlib(v);       // only the modules this host wants to expose
sq_pop(v, 1);

// compile and call something, then
sq_close(v);
```

Every VM opened with `sq_open` must be closed with `sq_close`. A host may hold
many VMs, and they share nothing. `sq_newthread` is different: it makes a friend
VM that shares the parent's globals and registry. A script-level
[thread](sym:types.Thread) is such a friend VM.

The standard library is not loaded by default. A host that does not register
`io` and `system` gives its scripts no way to reach the file system. This is the
sandbox, so choose the registered modules on purpose.

## Error conventions

Most functions return `SQRESULT`. Do not compare it with zero. Use the macros.
They exist because the encoding has changed before.

```cpp
if (SQ_FAILED(sq_getstring(v, -1, &s)))
  return report("expected a string");
```

A failure usually leaves the stack as it was, but not always. In a native
function, record `sq_gettop` on entry and restore it on an error path.

## Build configuration

- `_SQ64` makes `SQInteger` 64 bit. It is on by default, also on a 32-bit
  platform. `SQFloat` is a separate choice: `float`, or `double` with
  `SQUSEDOUBLE`.
- `NO_GARBAGE_COLLECTOR` removes the mark-and-sweep pass, described below.

## Memory management

Memory is reference counted. An optional mark-and-sweep collector finds the
cycles that reference counting cannot see. The collector never runs by itself.
The host calls `sq_collectgarbage` when it has time, so the VM has no pauses the
host did not ask for.

Building with `NO_GARBAGE_COLLECTOR` removes the collector and saves two pointers
per collectable object (tables, arrays, functions, threads, userdata and
generators). Then a reference cycle leaks, and the host has to break cycles
itself.

## Pages

Read [The stack](page:embedding/stack) first. Every other page here uses it.
[Sqrat bindings](page:embedding/bindings) is the shortcut: most hosts bind
through Sqrat and use the raw API only where Sqrat has no answer.

{{subtopics}}

## See also

- [C API reference](page:capi/index) - every exported function, grouped as the
  headers group them
- [The language](page:language/index) - syntax and semantics of the scripts
