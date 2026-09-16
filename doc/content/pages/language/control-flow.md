---
title: Control flow
group: Language
order: 40
summary: `if`, `while`, `for`, `foreach`, and the `switch` that is off by default.
---

Quirrel has the usual C-family statements: `if`/`else`, `while`, `do ... while`,
a C style `for`, `foreach`, and a `switch` that is off by default.

## if / else

A condition may be a plain expression, or a `local`/`let` declaration checked for
truth (`null`, integer `0` and float `0.0` are false; everything else is true). A
binding declared this way, optionally followed by `;` and a separate condition, is
scoped to the whole chain: every `else if` and the final `else` can still see it,
which is the reason to write it here instead of on the line above.

{{example:language/control-flow-if-basic}}

{{example:language/control-flow-if}}

## while and do ... while

`while` checks the condition before each pass, so the body may run zero times.
`do ... while` checks it after, so the body always runs at least once.

{{example:language/control-flow-while}}

## for

`stat := 'for' '(' [init] ';' [cond] ';' [step] ')' stat`. Any part may be empty,
which is how `for (;;) { ... }` spells an infinite loop. `init` and `step` accept
several comma-separated declarations or expressions, not only one.

{{example:language/control-flow-for}}

## foreach

`foreach` walks an array, a table, a class, a string, or a generator. The
one-variable form binds the value; the two-variable form binds a leading index or
key first. What that leading binding means depends on the container:

| Container | one variable | two variables |
| --- | --- | --- |
| array | element | position (from 0), element |
| table, class, instance | value | key, value |
| string | character code (an integer, not a one-char string) | position (from 0), character code |

{{example:language/control-flow-foreach-array}}
{{example:language/control-flow-foreach-table}}
{{example:language/control-flow-foreach-string}}

A table's iteration order is not guaranteed, so a program that must be
deterministic sorts the keys itself, as the table sample above does.

The bound variable(s) may also be a destructuring pattern instead of a plain
name; see [Destructuring](page:language/destructuring) for the pattern syntax
and defaults.

## break, continue, return

`break` leaves the innermost `for`, `foreach`, `while` or `do ... while` (or a
`switch`, see below). `continue` skips to the next pass of the innermost loop.
`return` leaves the whole function, not just a loop it happens to be inside.

{{example:language/control-flow-loop-control}}

## Loop bodies and closures

Each time a loop body runs, its own `local`/`let` declarations are fresh:
a closure created inside the body and kept past the loop sees the value that
binding held on the pass that created it. `foreach`'s own bound variables follow
the same rule, since a new value (or key/value pair) is handed to them each pass.

`for` is the exception. Its control variable lives in one slot for the entire
loop, reused and mutated by every pass; a closure that captures it directly keeps
reading that same slot, so once the loop ends every such closure observes the
final value, not the one from its own pass. Give it a fresh `local` inside the
body to capture the per-pass value instead.

{{example:language/control-flow-closures}}

## switch

`switch` is disabled by default: with no directive, `switch`, `case` and
`default` are ordinary identifiers and `switch (x) { ... }` fails to compile.
The `#allow-switch-statement` compiler directive turns the keywords back on,
scoped to the enclosing block, so it reverts at the closing brace. A leading
`#default:allow-switch-statement` sets it for the whole compilation unit
instead. See [Compiler directives](page:language/directives).

```nut
switch (x) {
  case 1:
    println("one")
}
```

A `case` without a `break` falls through into the next one, so several labels
can share a body by stacking them with no code in between. `default` (if
present) must be the last clause; a `case` after it fails to compile.

{{example:language/control-flow-switch}}

## Notes

- A `local`/`let` declared in an `if` condition, a `for` init, or bound by
  `foreach` does not leak past the statement's own body.
- `while` does not accept a declaration in its condition; only `if` does.
