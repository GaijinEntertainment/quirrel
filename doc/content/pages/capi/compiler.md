---
title: Compiler and static analysis
group: C API
order: 124
summary: Compiling source, and configuring the diagnostics that run with it.
---

Turning source into a closure, and configuring the diagnostics that run with it.

## Compiler

`sq_compile` is enough for most hosts. It takes a buffer and leaves a callable
closure on the stack. The AST entry points below it are for tools that want the
tree: a formatter, a coverage pass, an analyzer run without codegen. They follow
the pipeline the compiler uses internally: parse, analyze, translate.

{{capi:compiler}}

## Static analysis

Diagnostic state is process-wide, not per VM, so a host configures it once at
startup. A diagnostic can be addressed by name or by number.
`sq_printwarningslist` writes the full set.

{{capi:static analysis}}
