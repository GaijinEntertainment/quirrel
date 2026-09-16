---
title: Errors and exceptions
group: Language
order: 85
summary: `throw`, `catch`, and what an error carries with it.
---

An error is a value in flight. `throw` hands any value to the nearest
enclosing `catch`, and if there is none, it unwinds the whole script.

## throw and catch

`throw expr` can throw anything: a string, a number, a table built for the
occasion, or an instance of a class written to look like an error. There is
no built-in `Error` type to inherit from.

`try { ... } catch (e) { ... }` binds `e` to exactly the thrown value,
unwrapped and unconverted - `typeof e` after `throw 42` is `"integer"`, not
some wrapper type. A `catch` clause may be typed by naming a class before
the bound name, `catch (AmmoError e)`; it then only matches a thrown value
that is `instanceof` that class. Several typed clauses may follow one
`try`, tried in order, with a single untyped clause allowed as the
catch-all at the end.

{{example:language/errors-typedcatch}}

{{example:language/errors-throw}}

## No finally, and rethrowing

Quirrel has `try`/`catch` but no `finally`. Cleanup that must run either way
goes inline before the risky call, or in the `catch` block followed by `throw
e` to send the same value on to an outer handler.

{{example:language/errors-rethrow}}

## assert

[assert](sym:assert) throws if its first argument is false (by the same
rule as `if`: `null`, `false`, `0` and `0.0` count), with `"assertion
failed"` as the default message. Passing a function as the second argument
defers building the message: it is called, and its return value used as the
thrown value, only when the assertion actually fails. This keeps an
expensive diagnostic off the path where nothing is wrong.

{{example:language/errors-assert}}

## Uncaught errors

An error that reaches the top of the script without a matching `catch`
unwinds every frame and hands the value to the host's error handler instead
of the script. The standalone interpreter's default handler prints the
thrown value, a call stack, and the locals at each frame, then stops
running the script; an embedding host installs its own handler and decides
what "uncaught" means for it (log it, show it, ignore it).

A `throw` inside a script function that is itself running as a callback
from native code - a sort comparator, an event handler - crosses that
native frame normally. From the calling script's point of view, an ordinary
`try`/`catch` around the call that reached into native code catches it
exactly as if no native code had been involved.

{{example:language/errors-native-boundary}}

## Reading a "wrong type" error

The error people run into most is a type mismatch on a call, and it has one
shape whether the mismatch is a type-annotated parameter or a built-in
function's own argument check:

```
parameter 2 of 'heal' has an invalid type 'string' ; expected: 'integer'
```

The parameter index counts `this` as parameter 0, so parameter 1 is the
first argument written at the call site, parameter 2 the second, and so on.
`expected` lists every type the parameter accepts, `|`-joined for a union
annotation such as `int|null`.

{{example:language/errors-paramtype}}
