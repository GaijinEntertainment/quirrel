---
title: Generators and threads
group: Language
order: 65
summary: `yield`, `resume`, and the generator a call returns instead of a value.
---

A `yield` anywhere in a function's own body makes that function a **generator
function**. Nothing else marks it: no keyword decorates the declaration, and calling it
never runs the body. It returns a new generator instead, suspended before its first
instruction.

## Becoming a generator

`yield` belongs to the function that directly contains it, not to whatever that
function calls. A `yield` written inside a lambda or a `function` literal passed to
something else - a sort comparator, an `each` callback - makes that inner closure the
generator. The outer function that merely called `.each()` is not one, and never
returns a generator at all.

## States and resuming

[types.Generator.getstatus](sym:types.Generator.getstatus) reports one of
`"suspended"`, `"running"` or `"dead"`. A freshly created generator already reports
`"suspended"`, the same as one paused mid-body.

`resume gtor` is a keyword, not a method call: it moves the generator forward to its
next `yield`, or to a `return` or an uncaught exception. Either of those kills the
generator permanently - `"dead"` never goes back to `"suspended"` the way a
[thread](sym:types.Thread.getstatus) goes back to `"idle"` and can be restarted.
Resuming a dead generator throws `resuming dead generator`; resuming something that is
not a generator at all throws `trying to resume a '<type>', only generator can be
resumed`.

A `foreach` can drive a generator directly in place of an array or a table, resuming it
once per iteration until it dies. The value from its `return` is discarded either way.

{{example:language/generators-wave-spawner}}

## What yield evaluates to

`yield <expr>` hands `<expr>` out to whoever resumes the generator - the one job it
shares with a thread's [suspend](sym:suspend). There the similarity ends. `yield` is a
statement, not an expression: `let x = yield 1` fails to compile. `resume` takes no
argument either, so there is no way to hand a value back in, unlike
[wakeup](sym:types.Thread.wakeup), whose argument becomes `suspend`'s return value on
the other side.

A generator also has no VM of its own: it runs on its caller's own stack, not a
separate one the way [newthread](sym:newthread) gives a thread. Reaching for the base
`suspend()` inside a generator's body does not pause the generator - it reaches the
same root VM the generator itself is running on, so it throws the same `cannot suspend
the root vm` that calling it anywhere outside a thread does.

{{example:language/generators-patrol-route}}

## Edge cases

- A generator that calls `resume` on itself while it is running throws `resuming
  active generator`.
- A suspended generator keeps only a weak reference to its `this`; a running one keeps
  a strong one. Dropping every other reference to an object bound as `this` on a
  suspended generator lets the garbage collector reclaim it before the generator is
  ever resumed again.
- `clone` on a generator returns the same generator - see
  [types.Generator.clone](sym:types.Generator.clone).
