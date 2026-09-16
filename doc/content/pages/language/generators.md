---
title: Generators and threads
group: Language
order: 65
summary: `yield`, `resume`, generator states, and the differences from threads.
---

A `yield` anywhere in a function's own body makes that function a **generator
function**. No keyword marks the declaration. A call to a generator function does
not run the body. It returns a new generator, suspended before its first
instruction.

## Becoming a generator

`yield` belongs to the function that directly contains it. A `yield` inside a lambda
or a `function` literal that is passed to another function (a sort comparator, an
`each` callback) makes that inner closure the generator function. The outer function
that calls `.each()` is not a generator function and does not return a generator.

## States and resuming

[types.Generator.getstatus](sym:types.Generator.getstatus) reports one of
`"suspended"`, `"running"` or `"dead"`. A new generator reports `"suspended"`, the
same as one paused in its body.

`resume gtor` is a keyword, not a method call. It runs the generator to its next
`yield`, or to a `return` or an uncaught exception. A `return` or an uncaught
exception makes the generator `"dead"` permanently. A dead generator never goes back
to `"suspended"`. A [thread](sym:types.Thread.getstatus) is different: it goes back
to `"idle"` and can be restarted. Resuming a dead generator throws `resuming dead
generator`. Resuming a value that is not a generator throws `trying to resume a
'<type>', only generator can be resumed`.

A `foreach` can iterate over a generator in place of an array or a table. It resumes
the generator once per iteration until the generator dies. `foreach` discards
the value of the generator's `return`. `resume` does not: the `return` value
becomes the value of the `resume` expression, the same way a `yield` value
does.

{{example:language/generators-wave-spawner}}

## What yield evaluates to

`yield <expr>` passes `<expr>` to the code that resumes the generator. A thread's
[suspend](sym:suspend) does the same. The similarity ends there. `yield` is a
statement, not an expression: `let x = yield 1` fails to compile. `resume` takes no
argument, so there is no way to pass a value back into the generator.
[wakeup](sym:types.Thread.wakeup) is different: its argument becomes the return
value of `suspend` in the thread.

A generator has no VM of its own. It runs on the stack of its caller;
[newthread](sym:newthread) gives a thread a separate stack. A call to the base
`suspend()` inside a generator body does not pause the generator. It acts on the
root VM that runs the generator, so it throws `cannot suspend the root vm`, the same
as a call outside a thread.

{{example:language/generators-patrol-route}}

## Edge cases

- A generator that calls `resume` on itself while it is running throws `resuming
  active generator`.
- A suspended generator keeps only a weak reference to its `this`; a running one
  keeps a strong reference. When every other reference to the `this` object of a
  suspended generator is dropped, the garbage collector can reclaim the object
  before the generator is resumed.
- `clone` on a generator returns the same generator; see
  [types.Generator.clone](sym:types.Generator.clone).
