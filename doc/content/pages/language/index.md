---
title: The language
group: Language
order: 5
group_index: true
---

This chapter is the language itself: what the compiler accepts, and what each
construct means at runtime. It says nothing about the standard library - a function
you call is on its own page, one page per symbol, under Modules and Types.

Quirrel is a C-family language with dynamic types, closures, generators, classes
and reference-counted memory. If you already write Python, JavaScript, Lua or
Squirrel, start at [the introduction](page:index) instead: it maps what you know
onto these pages, so you can read only the parts that differ.

The pages are in reading order. Each one is short, states the rule, and shows a
sample that runs.

## Pages

{{subtopics}}

## The shape of the language

- Everything is a value: a function, a class, a generator and a module all sit in
  variables and get passed around.
- A name is bound once with [let](page:language/bindings), and the compiler rejects
  a second binding of it. `local` is there when a name has to change.
- A [table](page:language/containers) slot has to exist before `=` writes to it;
  `<-` is what creates one. This is the single rule most ported code trips over.
- A missing key throws rather than giving `null`, so `?.` and `??` are how an
  absent value is handled on purpose.
- There is no `undefined`, no automatic string-to-number conversion in arithmetic,
  and no `NaN` out of integer division: each of those is an error instead.

## See also

- [Cheat sheet](page:cheatsheet) - the same rules compressed onto two printable pages
- [Traps](page:cheatsheet#traps) - what the rules above cost when they are forgotten
- [Guides](page:guides) - attributes, limits, and the rest of the reference material
- [Embedding Quirrel](page:embedding/index) - the language seen from the C++ host
