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

The parameters are already on the stack when the function runs. Index 1 is
`this`, and the explicit parameters start at index 2. `sq_gettop` gives the
count, so a variadic native reads its arguments up to the top.

Free variables, if the closure has any, come after the explicit parameters and
are read the same way. They also count toward `sq_gettop`, so for such a
closure `sq_gettop` is larger than the argument count.

## Registering one

Use the declaration string form. It gives the function parameter names and
types. The VM enforces and reports them. Every signature on this site comes
from a declaration string:

```cpp
sq_pushroottable(v);
sq_new_closure_slot_from_decl_string(
    v, my_clamp, 0,
    "pure clamp(x: number, min: number, max: number): number",
    SQ_DOC("Clamps x to the range [min, max]"));
sq_pop(v, 1);
```

The older form, `sq_newclosure` plus `sq_newslot`, still works. A function
registered with a type mask uses it. A symbol bound that way reports its
parameters as `arg1`, `arg2`, and its page says so.

`SQ_DOC` wraps a docstring so a release build can drop the text.

## Reporting an error

`sq_throwerror` raises a value that behaves like a script `throw`, so a `try`
in the calling script catches it:

```cpp
if (sq_gettype(v, 2) != OT_STRING)
  return sq_throwerror(v, "expected a string");
```

`sq_throwparamtypeerror` produces the VM's own wrong-argument message. Use it
when the error is a parameter type, so the native's diagnostics read like the
built-in ones.

Return `SQ_ERROR` after throwing. Returning 0 leaves the error set but tells
the VM the call succeeded.

## Userdata and userpointers

`sq_newuserdata` allocates a block of a given size, pushes it as a value, and
returns a pointer to the payload. The VM owns the memory and frees it with every
other object. This is the way to attach a C struct to a value a script holds. A
userdata can have a delegate, and then it behaves like a table.

To learn when the userdata is freed, install a release hook:

```cpp
typedef SQInteger (*SQRELEASEHOOK)(HSQUIRRELVM vm, SQUserPointer, SQInteger size);
sq_setreleasehook(v, idx, my_release);
```

A **userpointer** is a bare `void *` passed by value. It has no allocation, no
delegate and no release hook. `sq_pushuserpointer` costs nothing. The host owns
the lifetime of the memory it points to.

## Runtime errors from script

When a script error reaches the top and nothing catches it, the VM calls the
error handler set by `sq_seterrorhandler`. `sq_seterrorhandler` pops a Quirrel
function off the stack. The handler receives an environment object and the
thrown value, which can be of any type. A debugger uses this hook to stop at
the throw, not after it.
