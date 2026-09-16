---
title: Compiler and static analysis
group: C API
order: 124
summary: Compiling source, and the diagnostics that run with it.
---

Turning source into a closure, and configuring the diagnostics that run on the way.

## Compiler

`sq_compile` is the whole job for most hosts: it takes a buffer and leaves a
callable closure on the stack. The AST entry points below it exist for tools that
want the tree - a formatter, a coverage pass, an analyzer run without codegen - and
they follow the pipeline the compiler uses internally: parse, analyze, translate.

{{capi:compiler}}

## Static analysis

Diagnostic state is process-wide, not per VM, so a host configures it once at
startup. Names and numbers both address a diagnostic; `sq_printwarningslist` writes
the full set.

{{capi:static analysis}}
