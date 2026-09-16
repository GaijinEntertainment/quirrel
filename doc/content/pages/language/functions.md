---
title: Functions
group: Language
order: 30
summary: Definitions, default and vararg parameters, calls and closures.
---

A function is a value. It can be stored in a table, passed to another function,
returned, and kept in an array, and it captures the bindings around it where it
was written.

## Declaring one

`function name(params) { ... }` binds a name in the current scope. A parameter may
have a default, and a trailing `...` collects the rest of the arguments into
`vargv`, an ordinary array. A default is evaluated once, when the closure is
created: a table or array default is one object that every call
without that argument shares, so do not mutate it.

Defaults may only come after the parameters without one, and a call must still
pass every parameter that has no default.

A parameter and the return value may also carry a declared type - see
[Type annotations](page:language/annotations).

{{example:language/functions-minimal}}

The same function written as a lambda takes the same arguments and returns the
same value; `@(params) expression` is just a shorter spelling for the one
expression case.

{{example:language/functions-basics}}

## Lambdas

`@(params) expression` is a function of one expression, whose value is the result.
There is no `return`.

The body is an expression, so braces around it make a table literal, not a
statement block. `@() {}` is a function returning an empty table, not a function
with an empty body. When a lambda gets long enough to want statements, write
`function` instead.

{{example:language/functions-lambdas}}

## this

A function called through a table or an instance receives that container as
`this`. Taken out of its container and called on its own, it does not: `this` is
then whatever the caller supplies. [bindenv](sym:types.Function.bindenv) attaches
a fixed one, and [call](sym:types.Function.call) passes one for a single call.

{{example:language/functions-this-basic}}

A method reached through the instance that owns it gets that instance as `this`.
Detach it from its container and `this` is whatever the caller supplies instead.

{{example:language/functions-this}}

## Attributes

A declaration can carry attributes in brackets, which tell the compiler what a
call may assume: `const function [pure] armorAt(angle) { ... }`. See
[Function attributes](page:attributes).

## Edge cases

- A parameter list is checked at the call. Too few or too many arguments throws
  `wrong number of parameters`, and the message counts `this`, so a two parameter
  function reports "3 required".
- Only a function declared with `...` gets its own `vargv`. A bare `vargv` in a
  function without `...` is an ordinary captured name that resolves to the nearest
  enclosing `...` function - the top-level script always counts as one - so it holds
  that outer function's extra arguments, not this call's.
- A named function expression, `let update = function updateSquad() {...}`, names
  the closure for stack traces without binding `updateSquad` in the enclosing
  scope.
- `clone` on a function returns the same function. Functions are immutable, so
  there is nothing to copy.
