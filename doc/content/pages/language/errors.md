---
title: Errors and exceptions
group: Language
order: 85
summary: `throw`, `try`/`catch`, `assert`, and uncaught errors.
---

`throw` passes any value to the nearest enclosing `catch`. If there is no
`catch`, it unwinds the whole script.

## throw and catch

`throw expr` can throw any value: a string, a number, a table, or an
instance of a user-defined error class. There is no built-in `Error` type to
inherit from.

`try { ... } catch (e) { ... }` binds `e` to the thrown value itself, with no
wrapper and no conversion. `typeof e` after `throw 42` is `"integer"`. A
`catch` clause may name a class before the bound name, `catch (AmmoError e)`.
It then matches only a thrown value that is `instanceof` that class. Several
typed clauses may follow one `try`. They are tried in order. One untyped
clause is allowed at the end as the catch-all.

{{example:language/errors-typedcatch}}

{{example:language/errors-throw}}

## No finally, and rethrowing

Quirrel has `try`/`catch` but no `finally`. Cleanup that must run in both
cases goes before the call that can throw, or in the `catch` block. In the
`catch` block, `throw e` after the cleanup sends the same value on to an
outer handler.

{{example:language/errors-rethrow}}

## assert

[assert](sym:assert) throws if its first argument is false (by the same
rule as `if`: `null`, `false`, `0` and `0.0` are false). The default message
is `"assertion failed"`. A function as the second argument delays the
message: `assert` calls it only when the assertion fails, and uses its
return value as the thrown value. An expensive diagnostic then costs nothing
when the assertion holds.

{{example:language/errors-assert}}

## Uncaught errors

An error that reaches the top of the script without a matching `catch`
unwinds every frame and passes the value to the host's error handler. The
default handler of the standalone interpreter prints the thrown value, a
call stack, and the locals of each frame, then stops the script. An
embedding host installs its own handler and decides what to do with an
uncaught error (log it, show it, ignore it).

A `throw` inside a script function that runs as a callback from native code
(a sort comparator, an event handler) crosses that native frame. An ordinary
`try`/`catch` around the call into native code catches it, the same as if no
native code was involved.

{{example:language/errors-native-boundary}}

## Reading a "wrong type" error

The most common error is a type mismatch on a call. It has one form for a
type-annotated parameter and for the argument check of a built-in function:

```
parameter 2 of 'heal' has an invalid type 'string' ; expected: 'integer'
```

The parameter index counts `this` as parameter 0, so parameter 1 is the
first argument written at the call site, parameter 2 the second, and so on.
`expected` lists every type the parameter accepts, `|`-joined for a union
annotation such as `int|null`.

{{example:language/errors-paramtype}}
