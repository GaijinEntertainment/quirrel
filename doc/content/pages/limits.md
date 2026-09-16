---
title: Implementation limits
group: Guides
order: 95
summary: The ceilings the bytecode imposes, and the errors they report.
---

The compiler refuses a few shapes of code that the bytecode cannot express. Each
one reports itself clearly, but the message says nothing about the ceiling behind
it, so this page collects them.

None of these is a limit on how large a script may be. They are all per function,
and splitting the work into smaller functions clears every one of them.

## Stack slots per function

A function has **255** stack slots. `this` takes the first, then the parameters,
then the locals, then whatever temporaries an expression needs while it is being
evaluated. They all come from the same budget, so a function with many parameters
has room for fewer locals.

```
too many function stack slots: cannot allocate local 'v254' at slot 255;
bytecode supports at most 255 slots per function
```

The other wording appears when the slot number itself cannot be encoded, because
`0xFF` is reserved as a marker:

```
too many function stack slots: bytecode supports at most 255 slots per function;
slot 255 cannot be encoded because 0xFF is reserved
```

In practice a function that runs out of slots is a function that wanted to be
several functions. A generated one that genuinely needs hundreds of values should
hold them in an array or a table, which costs a single slot.

## Nesting depth

The parser carries one depth budget of **500** units for the whole source tree, and
reports the same message whatever exhausts it:

```
AST too big. Consider simplifying it
```

A unit is a grammar rule entered, not a level of nesting as it appears in the text,
so the depth a given construct buys varies a lot. Nesting an expression in
parentheses walks the full precedence chain each time and costs about fifteen units
per level, while a statement block costs about two. Measured against the current
compiler:

| construct | levels that still compile |
| --- | --- |
| parenthesised expression, `((((1))))` | about 33 |
| nested array or table literal | about 33 |
| nested `if` | about 157 |
| nested statement block, `{ { { } } }` | about 236 |

Hand-written code does not come near any of these. Generated code does: a rule
compiler that emits one nested `if` per condition, or an expression builder that
parenthesises defensively at every step, hits the expression figure quickly. Emit a
flat chain, or a table the runtime walks, instead of a deeper tree.

## Active catch clauses per function

The bytecode holds at most **255** simultaneously active traps in one function, so
that many nested `try` blocks:

```
too many active catch clauses: bytecode supports at most 255 simultaneously
active traps per function
```

Nesting `try` that deeply exhausts the parser's depth budget first, so this message
is hard to reach. It is here because a bytecode generator that emits traps without
matching source nesting can still hit it.

## Static memos per function

A function may contain **32768** distinct
[static](page:language/operators#the-static-memoisation-operator) expressions, which
is the width of the index field in the instruction.

```
too many static memos in function
```

Only generated code approaches this. Note that the limit counts places in the code,
not evaluations: a `static` inside a loop is one memo however many times the loop
runs.
