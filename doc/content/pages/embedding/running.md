---
title: Compiling and calling
group: Embedding
order: 104
summary: Compiling a source buffer, and calling what it produced.
---

## Compiling

`sq_compile` turns a source buffer into a closure and pushes it:

```cpp
SQRESULT sq_compile(HSQUIRRELVM v, const char *s, SQInteger size,
                    const char *sourcename, SQBool raiseerror,
                    const HSQOBJECT *bindings = nullptr);
```

`sourcename` is the name that appears in a runtime error and a stack trace, so it
should be the path a reader can open even when the source came from an archive.
`bindings` supplies compile-time names, which is how a host makes constants and
imported modules visible to the unit being compiled.

Compiling does not run anything. The closure it leaves on the stack has to be
called, exactly like any other function, and calling it is what executes the
top-level statements of the file.

A syntax error goes to the compiler error handler, which is set once per VM:

```cpp
typedef void (*SQCOMPILERERROR)(HSQUIRRELVM v, SQMessageSeverity severity,
                                const char *desc, const char *source,
                                SQInteger line, SQInteger column,
                                const char *extra_info);

sq_setcompilererrorhandler(v, my_handler);
```

The severity is what separates an error from an analyzer warning, so one handler
covers both, and `extra_info` carries the diagnostic's own context where it has any.
`sqstd_seterrorhandlers` installs a handler that prints all of this, which is enough
for most hosts and is the fastest way to stop losing errors silently.

## Calling

A call is assembled on the stack: the closure, then its `this`, then the arguments,
then `sq_call` with the total count including `this`.

```cpp
sq_pushroottable(v);
sq_pushstring(v, "foo", -1);
sq_get(v, -2);              // the closure

sq_pushroottable(v);        // 'this' for the call
sq_pushinteger(v, 1);
sq_pushfloat(v, 2.0);
sq_pushstring(v, "three", -1);

sq_call(v, 4, SQFalse, SQFalse);
sq_pop(v, 2);               // the closure and the root table
```

That is `foo(1, 2.0, "three")`. The count is 4 because `this` is one of them, which
is the mistake worth checking first when a call reports the wrong arity.

`sq_call` pops the arguments and the closure's `this`, and pushes the return value
only when the `retval` argument asks for it. The last argument decides whether a
failure runs the VM error handler on the way out; pass false when the caller intends
to report the failure itself, or the error is printed twice.

If the script throws and nothing catches it, `sq_call` fails. Read the thrown value
with `sq_getlasterror`.

## Tail calls

`sq_tailcall` replaces the current native frame with the call instead of nesting it,
which is what keeps a native that dispatches to a script function from growing the
stack.
