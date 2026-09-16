---
title: Embedding Quirrel
group: Embedding
order: 100
group_index: true
---

Quirrel is an extension language: the compiler and the VM are a C library, and a
host application drives them. Everything needed to do that is declared in
`squirrel.h`, with the standard modules in the `sqstd*.h` headers beside it.

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

Every VM opened with `sq_open` must be closed with `sq_close`. A host may hold many,
and they share nothing. `sq_newthread` is different: it makes a friend VM that
shares the parent's globals and registry, which is what a script-level
[thread](sym:types.Thread) is.

Nothing in the standard library is loaded on its own. A host that never registers
`io` and `system` has given its scripts no way to touch the file system, whatever
they write. That is the whole sandbox, and it is worth deciding deliberately rather
than by copying a startup sequence.

## Error conventions

Most functions return `SQRESULT`. It is not an integer to compare against zero: use
the macros, both of which exist because the encoding has changed before.

```cpp
if (SQ_FAILED(sq_getstring(v, -1, &s)))
  return report("expected a string");
```

A failure usually leaves the stack as it was, but not always, and the safe habit in
a native function is to record `sq_gettop` on entry and restore it on the way out of
an error path.

## Build configuration

- `_SQ64` makes `SQInteger` 64 bit; it is on by default, even on a 32-bit
  platform. `SQFloat` is a separate choice - `float` unless `SQUSEDOUBLE` makes it
  `double`.
- `NO_GARBAGE_COLLECTOR` drops the mark-and-sweep pass, described below.

## Memory management

Memory is reference counted, with an optional mark-and-sweep collector on top for
the cycles that reference counting cannot see. The collector never runs by itself:
the host calls `sq_collectgarbage` when it has time, which is what keeps the VM free
of pauses it did not ask for.

Building with `NO_GARBAGE_COLLECTOR` removes the collector, and saves two pointers
per collectable object (tables, arrays, functions, threads, userdata and
generators). Then a reference cycle leaks, and the host is responsible for breaking
them.

## Pages

[The stack](page:embedding/stack) is the mechanism every other page here uses, so
read that one first. [Sqrat bindings](page:embedding/bindings) is the shortcut: most
hosts bind through it and touch the raw API only where Sqrat has no answer.

{{subtopics}}

## See also

- [C API reference](page:capi/index) - every exported function, grouped as the
  headers group them
- [The language](page:language/index) - what the scripts your host runs may say
