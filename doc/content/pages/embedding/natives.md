---
title: Native functions
group: Embedding
order: 106
summary: Writing a C function a script can call.
---

A native function has one prototype:

```cpp
typedef SQInteger (*SQFUNCTION)(HSQUIRRELVM);
```

The return value is a count, not a status:

- **1** - one value was pushed and is the result.
- **0** - nothing was pushed; the call evaluates to null.
- **SQ_ERROR** - an error was raised, usually by `sq_throwerror` just before.

Its parameters are already on the stack when it runs. Index 1 is `this`, and the
explicit parameters follow from 2. `sq_gettop` gives the count, so a variadic native
reads its arguments by walking to the top.

Free variables, if the closure has any, sit after the explicit parameters and are
read the same way. They also count toward `sq_gettop`, which is worth remembering
before treating that number as the arity.

## Registering one

The declaration string is the form to use. It gives the function real parameter
names and types, which the VM then enforces and reports, and which is where every
signature on this site comes from:

```cpp
sq_pushroottable(v);
sq_new_closure_slot_from_decl_string(
    v, my_clamp, 0,
    "pure clamp(x: number, min: number, max: number): number",
    SQ_DOC("Clamps x to the range [min, max]"));
sq_pop(v, 1);
```

The older form, `sq_newclosure` plus `sq_newslot`, still works and is what a
function registered with a type mask uses. The cost shows up on this site: a symbol
bound that way reports its parameters as `arg1`, `arg2`, and its page has to say so.

`SQ_DOC` wraps a docstring so a release build can drop the text.

## Reporting an error

`sq_throwerror` raises a value that behaves exactly like a script `throw`, so a
`try` in the calling script catches it:

```cpp
if (sq_gettype(v, 2) != OT_STRING)
  return sq_throwerror(v, "expected a string");
```

`sq_throwparamtypeerror` produces the VM's own wrong-argument message instead, which
is what to use when the complaint is a parameter type, so a native's diagnostics
read like the built-in ones.

Return `SQ_ERROR` after throwing. Returning 0 leaves the error set but tells the VM
the call succeeded.

## Userdata and userpointers

`sq_newuserdata` allocates a block of a given size, pushes it as a value, and
returns a pointer to the payload. The VM owns the memory and frees it with every
other object, so this is the way to attach a C struct to something a script holds.
A userdata can be given a delegate, and then it behaves like a table.

To learn when it goes away, install a hook:

```cpp
typedef SQInteger (*SQRELEASEHOOK)(HSQUIRRELVM vm, SQUserPointer, SQInteger size);
sq_setreleasehook(v, idx, my_release);
```

A **userpointer** is the other kind: a bare `void *` passed by value, with no
allocation, no delegate and no release hook. `sq_pushuserpointer` costs nothing, and
the lifetime of whatever it points at is entirely the host's problem.

## Runtime errors from script

When a script error reaches the top with nothing catching it, the VM calls the error
handler set by `sq_seterrorhandler`, which pops a Quirrel function off the stack. The
handler receives an environment object and the thrown value, which can be of any
type. This is the hook a debugger uses to stop at the throw rather than after it.
