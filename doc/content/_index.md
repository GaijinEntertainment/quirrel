Quirrel is a small imperative, object-oriented scripting language built to be
embedded in a C++ application. It has C-like syntax, dynamic types, closures,
generators, coroutines and reference-counted memory, and it is designed to fit the
size and latency budget of a game rather than of a server.

It began as [Squirrel](http://www.squirrel-lang.org/) and is not compatible with
it. The changes are almost all in one direction: make a mistake fail at compile
time rather than at runtime.

{{example:tour}}

## Coming from another language

Quirrel is close enough to the four languages below that most of what you know
transfers on the first day. Each page maps the constructs you already write, and
ends with the traps that catch people from that language.

{{subtopics}}

## What is on this site

Every signature here is read out of a running VM rather than typed by hand, so a
page cannot describe a function the implementation does not have. Every sample is
checked in with its output, runs in CI, and runs again in your browser when you
press **Run this code** - edit it first if you want to see what changes.

The reference is grouped by how you reach a thing, because the same name can be
both. **Globals** are in the base library and need no import. **Module functions**
live in a module and have to be imported. **Type methods** belong to a value and
are called on it with a dot:

```nut
println(type(x))            // a global

from "math" import clamp    // a module function
let health = clamp(hp, 0, 100)

"hello".toupper()           // a type method
```

`strip` is a function in the [string](sym:string) module and a method on every
[string value](sym:types.String), so the two are separate entries here. One trap
follows from it: a table's own slots share a namespace with its type methods, and a
slot wins. `.$` reaches the type method and never looks at the slots, so use it on
any data you did not build yourself; see
[the type-method operator](page:language/operators#the-type-method-operator).

A page marked "This page has no written reference yet" is one the VM reports but
nobody has described. That is deliberate: a gap you can see is better than a gap
you cannot.

## Where to start

Each chapter opens with a page that says what is in it: [The
language](page:language/index) and [Guides](page:guides) above the modules and
types, then [Embedding Quirrel](page:embedding/index) and the [C API
reference](page:capi/index) below them. Inside a chapter, the pages are in reading
order.

- [Lexical structure](page:language/lexical) and
  [Values and types](page:language/types), if you are new to the language.
- [Bindings and constants](page:language/bindings), for `let`, `const` and why
  Quirrel prefers them to `local`.
- [Modules](page:language/modules), for `require`, `import` and how a script is
  loaded at all.
- [Embedding Quirrel](page:embedding/index), for calling into a VM from C++ and
  exposing your own types to script.

Press **/** anywhere to search. The search box also finds keywords, operators,
metamethods and the sections inside a page.

## See also

- [Cheat sheet](page:cheatsheet) - the whole language on two printable pages
- [Traps](page:cheatsheet#traps) - the mistakes that cost people an afternoon
- [Function attributes](page:attributes) and
  [Implementation limits](page:limits) - `pure`, `fastcall`, and the ceilings the
  bytecode has
- [Performance](page:performance) - Quirrel against Lua, LuaJIT, Luau, QuickJS and
  Squirrel 3, with the numbers committed
- [C API reference](page:capi/index) - the same library seen from C++

Every module and type follows, one page per symbol.
