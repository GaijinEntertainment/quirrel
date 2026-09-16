---
title: Compiler directives
group: Language
order: 88
summary: The `#` lines that turn a language feature on or off.
---

A line starting with `#` is a compiler directive: an identifier-like word,
optionally prefixed `default:`, that turns a language feature on or off for
the rest of the current unit. It is not a comment (see
[Lexical structure](page:language/lexical)) and it is not an `#if`-style
preprocessor - there is no expression, no macro, nothing conditional.

- `#strict` / `#relaxed` - root-table access and `delete`, forbidden together.
- `#forbid-root-table` / `#allow-root-table` - whether `::name` may reach the root table.
- `#forbid-delete-operator` / `#allow-delete-operator` - whether the `delete` operator may be used.
- `#forbid-clone-operator` / `#allow-clone-operator` - whether the `clone` operator may be used.
- `#forbid-switch-statement` / `#allow-switch-statement` - whether `switch`/`case`/`default` parse as a statement.
- `#forbid-implicit-type-methods` / `#allow-implicit-type-methods` - whether a missing slot falls back to a type method.
- `#forbid-auto-freeze` / `#allow-auto-freeze` - whether array/table/class literals get frozen automatically.
- `#forbid-compiler-internals` / `#allow-compiler-internals` - whether a `$${ ... }` block may be used as an expression.
- `#disable-optimizer` / `#enable-optimizer` - whether the bytecode peephole optimizer runs.

Four features start off and need an explicit `allow` (or `enable`) to turn
on: the `delete` operator, the `switch` statement, `$${ ... }` blocks, and
auto-freezing literals. The other four start on and need an explicit
`forbid` (or `disable`) to turn off: root-table access, the `clone`
operator, implicit type methods, and the bytecode optimizer.

`#strict` and `#relaxed` only ever touch root-table access and `delete` -
they do not touch `switch`, auto-freeze, or any of the others, whatever
their names suggest.

## Scope

Without `default:`, a directive applies from where it is written to the end
of the block that contains it - a function body or a bare `{ }` - and its
old value comes back once that block ends. At the top level, "the block" is
the rest of the file.

Three of the directives do not follow that rule: `#disable-optimizer` /
`#enable-optimizer`, `#forbid-implicit-type-methods` /
`#allow-implicit-type-methods`, and `#forbid-auto-freeze` /
`#allow-auto-freeze` are resolved while generating code for the enclosing
function, not while parsing the block that declares them, so their
effect lasts until that function ends (or the file ends, at the top level)
even from inside a nested `{ }` that has already closed.

{{example:language/directives-scope}}

## default:

Prefixing a directive with `default:` additionally changes the VM's default
for every unit compiled afterward in the same process - not just the rest
of the current file, but a module it `require`s later, too:

```nut
// main.nut
#default:forbid-root-table
let cfg = require("config.nut")   // config.nut inherits the ban, even
                                   // though it has no directive of its own
```

Without `default:`, a directive never crosses into a separately compiled
file; each file's parser starts from whatever the VM's current default is.

## Root table access

```nut
#forbid-root-table
::spawnPoint <- "hangar"
// Access to root table is forbidden
```

Reaching the root table through `::` is easy to do by accident and easy to
regret, since anything placed there is visible from every other unit that
shares the VM.

## delete and clone

`delete` is forbidden by default; `#allow-delete-operator` turns it back
on. `clone` is the opposite way round: it is allowed by default, and
`#forbid-clone-operator` is what turns it off. See
[Operators and expressions](page:language/operators) for what each
operator does and why `rawdelete` is the usual replacement for `delete`.

## switch

`switch` is off by default, which is why it needs
[`#allow-switch-statement`](page:language/control-flow) before `switch`,
`case` and `default` parse as anything other than identifiers.

## Implicit type methods

By default, indexing a table or instance with a name it does not have falls
back to a type method of that name - `params.filter` is
[filter](sym:types.Table.filter) when `params` has no `filter` slot.
`#forbid-implicit-type-methods` turns that fallback off, so a missing slot
is always a plain "index does not exist" error instead of silently handing
back a function:

```nut
#forbid-implicit-type-methods
let params = { name = "alpha" }
println(params.len)
// the index 'len' (type='string') does not exist
```

## Auto-freezing literals

`#allow-auto-freeze` wraps an array, table or class literal in an implicit
[freeze](sym:freeze) at the point it is built, so `let squad = { hp = 100
}` comes out already immutable - check with
[is_frozen](sym:types.Table.is_frozen). It composes with everything on
[Bindings and constants](page:language/bindings): the binding can still be
`let` or `local`, only the value it points at becomes read-only.

## Compiler internals

`$${ statements }` is a statement block used as an expression: its value is
whatever a `return` inside it produces. It exists for code that needs to
run more than one statement where only an expression is allowed, and it is
forbidden unless `#allow-compiler-internals` is in effect.

{{example:language/directives-compiler-internals}}

## The optimizer

`#disable-optimizer` turns off the peephole passes that fold constant
arithmetic and collapse jump chains in the generated bytecode. It changes
the size and shape of the bytecode, not what a script computes or prints -
there is nothing to observe from the running script either way, so reach
for it only while inspecting a `-bytecode-dump` or narrowing down a
suspected optimizer bug.

## #pos

`#pos:<line>:<column>` is not in the list above - it does not go through
`default:` and it cannot be turned off. It resets the compiler's line and
column counters to the given position, so every error and every
[`__LINE__`](page:language/lexical) after it is reported against that
position instead of counting lines in the real file. It exists for tools
that generate Quirrel source and want error messages to point at their own
input rather than at the generated text.
