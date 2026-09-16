---
title: Destructuring
group: Language
order: 60
summary: Unpacking a table by key or an array by position.
---

A `local` or `let` declaration can unpack a table's keys or an array's positions
straight into named bindings, instead of pulling out each one by hand.

## Table by key, array by position

`let { a, b } = table` binds `a` and `b` from the keys of the same name.
`let [a, b] = array` binds them from positions 0 and 1. `local` destructures the
same way. Commas between fields are optional either way.

{{example:language/destructuring-basics}}

## Defaults, and a key with no default

`let { a = 0 } = table` binds `a` from the key `a`, falling back to the default
expression only if that key is missing; array patterns take a default the same
way for a missing position. A field with no default throws if its key or
position is missing - it does not silently become `null`.

{{example:language/destructuring-defaults}}

A default only fires when the key or position is missing. A key that is present
and holds `null` is not missing, so it binds `null` and the default is not used.

## Type hints in a pattern

A field may carry a type hint, written the same way as a parameter annotation. A
hint and a default can appear together, in that order.

{{example:language/destructuring-hints}}

The hint is checked against the value that actually arrives, so it throws
`type 'string' differs from the declared type 'int'` rather than binding the wrong
type quietly. Combined with the rule above, a present `null` reaches the hint and is
rejected by it: `{ crew : int = 2 }` does not turn a `null` crew into `2`. See
[Type annotations](page:language/annotations).

## There is no rename syntax

Unlike JavaScript, a table pattern cannot bind a key under a different name.
`{ key }` always binds a variable named `key`; writing `=` after a field name
supplies its default, not an alias:

```nut
// this does NOT rename hitPoints to hp - it tries to use an undeclared
// variable named hp as hitPoints's default, and fails to compile
let { hitPoints = hp } = vehicle
```

To bind under a different name, destructure normally and then assign:
`let { hitPoints } = vehicle; let hp = hitPoints`. The `from "module" import`
statement below is the one place Quirrel does rename directly, with `as`.

## A pattern is one level deep

A field inside `{ }` or `[ ]` is always a plain name, typed or defaulted - never
another `{ }` or `[ ]` pattern. There is no nested destructuring. To reach into
a nested table or array, destructure one level, then destructure again:

{{example:language/destructuring-nested}}

## Destructuring in a from-import

`from "module" import a, b as c` pulls specific exports out of a module by
name, with an optional `as` to bind one under a different local name; `*`
imports everything. It is the selective, renaming counterpart to `import
"module"`, which binds the whole module under one name instead. This is the
form most game code uses to pull in a handful of names from a shared module.

{{example:language/destructuring-import}}

## Destructuring in a foreach

A `foreach` binder is a pattern position, so the fields of each element can be bound
directly instead of through a temporary. An index or key may still come first, and an
array element destructures by position.

{{example:language/destructuring-foreach}}

## Where else a pattern is accepted

A pattern is accepted in a `let`/`local` declaration, in a `foreach` binder, in a
`from ... import` list, and in a function parameter list - see
[Functions](page:language/functions). It is not accepted anywhere else: a bare
`{ name, strength } = squad` reassignment does not parse, because a pattern only
introduces new bindings.
