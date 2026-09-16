Quirrel is a small imperative, object-oriented scripting language for embedding
in a C++ application. It has C-like syntax, dynamic types, closures, generators,
coroutines and reference-counted memory. It is designed for the size and latency
budget of a game, not of a server.

It began as [Squirrel](http://www.squirrel-lang.org/) and is not compatible with
it. Most of the changes make a mistake fail at compile time instead of at runtime.

{{example:tour}}

## Coming from another language

Quirrel is close to the four languages below. Each page maps the constructs you
already write to their Quirrel form, and ends with the traps that catch people
from that language.

{{subtopics}}

## What is on this site

Every signature on this site is read from a running VM, so a page cannot describe
a function the implementation does not have. Every sample is checked in with its
output and runs in CI. It also runs in your browser when you press
**Run this code**, and you can edit it first.

The reference is grouped by how you reach a symbol, because the same name can be
more than one thing. **Globals** are in the base library and need no import.
**Module functions** live in a module and have to be imported. **Type methods**
belong to a value and are called on it with a dot:

```nut
println(type(x))            // a global

from "math" import clamp    // a module function
let health = clamp(hp, 0, 100)

"hello".toupper()           // a type method
```

`strip` is a function in the [string](sym:string) module and also a method on
every [string value](sym:types.String), so it has two entries here. One trap
follows from this: a table's own slots share a namespace with its type methods,
and a slot wins. `.$` calls the type method and ignores the slots, so use it on
data you did not build yourself. See
[the type-method operator](page:language/operators#the-type-method-operator).

A page marked "This page has no written reference yet" exists in the VM but has
no description yet.

## Where to start

Each chapter opens with an overview page: [The language](page:language/index)
and [Guides](page:guides) above the modules and types, then
[Embedding Quirrel](page:embedding/index) and the
[C API reference](page:capi/index) below them. Inside a chapter, the pages are in
reading order.

- [Lexical structure](page:language/lexical) and
  [Values and types](page:language/types), if you are new to the language.
- [Bindings and constants](page:language/bindings), for `let`, `const` and why
  they are preferred to `local`.
- [Modules](page:language/modules), for `require`, `import` and how a script is
  loaded.
- [Embedding Quirrel](page:embedding/index), for calling into a VM from C++ and
  exposing your own types to script.

Press **/** to search. The search box also finds keywords, operators,
metamethods and the sections inside a page.

## See also

- [Cheat sheet](page:cheatsheet) - the language on two printable pages
- [Traps](page:cheatsheet#traps) - common mistakes
- [Function attributes](page:attributes) and
  [Implementation limits](page:limits) - `pure`, `fastcall`, `nodiscard`, and
  the per-function limits of the bytecode
- [Performance](page:performance) - benchmarks against Lua, LuaJIT, Luau, QuickJS
  and Squirrel 3
- [C API reference](page:capi/index) - every exported C function

Every module and type follows, one page per symbol.
