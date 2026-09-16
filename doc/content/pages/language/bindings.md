---
title: Bindings and constants
group: Language
order: 20
summary: `local`, `let`, `const` and `enum`, and the scope each one gets.
---

Quirrel has four ways to bind a name: `local`, `let`, `const` and `enum`. All
four are scoped to the block where they are written, from the declaration to
the end of that block.

## let and local

`local` declares a variable that may be reassigned. `let` declares a binding
that may not: after its one initializing assignment, a later write to it is a
compile error. Default to `let`; reach for `local` only for a variable you
actually intend to reassign, such as a counter or an accumulator.

A `let` binding being fixed says nothing about the object it names. `let
ammoBelt = []` still allows `ammoBelt.append(...)` afterwards - only
`ammoBelt = otherArray` is rejected. [freeze](sym:freeze) is the tool for
locking the object itself; see below.

`let name` with no initializer forward-declares the binding: exactly one
later statement in the same block must define it, which is what lets two
closures capture each other before either is assigned.

{{example:language/bindings-basic}}

`local` is a variable in the usual sense. `let` names a value once and refuses a
second assignment, which is why it is the default choice: most bindings never
need to change, and saying so lets both the reader and the compiler rely on it.

{{example:language/bindings-let-local}}

A binding may also carry a declared type - see
[Type annotations](page:language/annotations).

Reading a forward-declared `let` before its definition runs is a compile
error, and so is a second assignment:

```nut
let target
println(target)  // error: binding 'target' cannot be used before its definition
target = "tank"
target = "plane" // error: a 'let' binding accepts a single assignment
```

## Forward declaration

A `let` may be declared with no value and defined later, on its own line. That is
the only way to write two functions that call each other, since whichever one is
written first would otherwise name something that does not exist yet.

{{example:language/bindings-forward}}

The compiler tracks the definition rather than trusting the author, so each way of
getting it wrong has its own message:

| Mistake | What the compiler says |
| --- | --- |
| declared but never defined | `forward declaration 'name' is never defined` |
| read before its definition | `binding 'name' cannot be used before its definition` |
| defined twice | `binding 'name' is already defined; a 'let' binding accepts a single assignment (declare it with 'local' to allow more)` |

The last one is the difference from `local`: a forward declared `let` still accepts
exactly one assignment, so it stays a binding that never changes after it is set.
Use `local` when the value really does need to be reassigned.

## const and global const

`const` binds a compile-time value: a number, string, `null`, or a table or
array nested from those. Every place the name is used, the compiler
substitutes the value directly - there is no variable, no stack slot and no
lookup left at runtime. A function may be `const` too, as long as it captures
no outer variable.

Plain `const` is purely lexical and leaves no trace at runtime; not even
[getroottable](sym:getroottable) sees it. `global const` additionally writes
the name into the shared `consttable`, reachable through
[getconsttable](sym:getconsttable), which is how a constant is meant to be
visible beyond the file that declares it.

{{example:language/bindings-const}}

### What the compiler will evaluate

The value is not limited to a literal. Anything the compiler can work out on its
own is allowed, and the result is what gets substituted:

| A `const` may be | Example |
| --- | --- |
| a simple value | `const MAGAZINE = 30` |
| a table or array of simple values, nested | `const LOADOUT = { belts = [1, 2, 3] }` |
| another constant, or a field reached from one | `const SECOND = LOADOUT.belts[1]` |
| an arithmetic, logical or conditional expression | `const RESERVE = MAGAZINE * 4` |
| a call to a pure function | `const CAP = max(MAGAZINE, 25)` |
| a function declaration that captures nothing | `const function armorAt(angle) { ... }` |

{{example:language/bindings-const-folding}}

The pure-function case is the one worth knowing. `max(MAGAZINE, 25)` is not
called at runtime; the compiler runs it once while compiling and stores the
answer. That is exactly what the [pure](page:attributes) attribute is for, and
the compiler enforces the connection: a call to anything not marked pure is
rejected with `Only calls to pure functions are allowed in constant
expressions`, so `const RANDOM = rand()` will not compile.

Because the substitution happens at compile time, a `const` costs nothing to
read, cannot be reassigned by any code anywhere, and is available in places a
runtime value is not - inside another `const`, or as an `enum` member's value.
The trade is that its value must be knowable without running the program, and
that a change to it requires recompiling every file that used it.

## enum and global enum

`enum` groups related constants under one name, accessed as `Enum.member`.
A member with no `=` gets an integer automatically; a member with `=` takes
an integer, a float or a string literal. Like `const`, a plain `enum` is
lexical only; `global enum` also lands in the consttable.

The automatic values are simpler than they look, and worth checking once: the
counter that fills them in starts at 0 and advances only when it is used, so
it counts only the automatic members, in their own order, ignoring any
explicit value written between them.

{{example:language/bindings-enums}}

## Scope and shadowing

A `local`, `let`, `const` or `enum` lives from its declaration to the end of
its own block, same as in most languages. What is not the same: within one
function, a nested block may not declare a new binding under a name already
used earlier in that function, by any of the four kinds, even a name from a
block that has since closed. The compiler rejects it as a conflict rather
than shadowing it.

Two independent (non-nested) blocks may each use the same name freely, since
neither is inside the other. A function body is its own scope regardless of
nesting, so a parameter may reuse a name from every enclosing scope without
conflict - that is the only place ordinary shadowing happens.

{{example:language/bindings-scope}}

Nesting the same name one block deeper is rejected, whatever kind either
declaration is:

```nut
let squadSize = 4
{
  let squadSize = 8  // error: conflicts with existing local variable
}
```

## freeze

[freeze](sym:freeze) marks a table, array, instance, class or userdata as
immutable and hands back a reference to it. The immutable flag lives on that
reference, not on the object: any other reference to the same object made
before the freeze - a variable it was copied to, a value already stored
elsewhere - still writes through normally, because it was never marked.
Content is shared, so a write through such a reference is visible from the
frozen one too; only the reference that was itself returned by `freeze`
refuses to write.

{{example:language/bindings-freeze}}

Assign the result back over the original name (`t = freeze(t)`) when every
access to an object should go through a frozen reference; keeping a separate
mutable alias around defeats the point.
