---
title: The language
group: Language
order: 5
group_index: true
---

This chapter describes the language: what the compiler accepts, and what each
construct does at runtime. The standard library is not here. Each function has
its own page under Modules and Types.

Quirrel is a C-family language with dynamic types, closures, generators, classes
and reference-counted memory. If you already write Python, JavaScript, Lua or
Squirrel, start at [the introduction](page:index). It maps what you know onto
these pages, so you can read only the parts that differ.

The pages are in reading order. Each one is short, states the rule, and shows a
sample that runs.

## Pages

{{subtopics}}

## The shape of the language

- Everything is a value. A function, a class, a generator and a module can all
  be stored in variables and passed around.
- A name is bound once with [let](page:language/bindings), and the compiler
  rejects a second assignment to it. Use `local` when a name has to change.
- A [table](page:language/containers) slot has to exist before `=` writes to it.
  `<-` creates a slot. Most ported code trips over this rule.
- A missing key throws. It does not give `null`. Use `?.` and `??` to handle an
  absent value.
- There is no `undefined`, no automatic string-to-number conversion in
  arithmetic, and no `NaN` from integer division. Each of those is an error.

## See also

- [Cheat sheet](page:cheatsheet) - the same rules on two printable pages
- [Traps](page:cheatsheet#traps) - common mistakes
- [Guides](page:guides) - attributes, limits and other reference material
- [Embedding Quirrel](page:embedding/index) - running the VM from a C++ host
