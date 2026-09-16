---
title: Modules
group: Language
order: 75
summary: `require`, `from`-`import`, and what a module file returns.
---

A module is a `.nut` file loaded through `require` or `import` rather than run directly. Its code
runs once, with `this` set to `null` rather than to a table, and whatever it `return`s
becomes its exports - the value every later `require`/`import` of the same file gets back
without running the file again.
Everything else the file declared stays private to it.

## require and require_optional

`require("modname")` reads, compiles and runs the named file the first time it is
requested, then returns its exports; a second `require` of the same name returns the
same exports without running the file again. A missing file throws.
`require_optional("modname")` is the same, except a missing file returns `null`
instead of throwing - the right choice for an optional dependency such as a mod or a
plugin that may not be installed.

A module and the file that uses it:

```nut weapons.nut
let MAX_AMMO = 240

function reload(state) {
  return state.__merge({ ammo = MAX_AMMO })
}

function isEmpty(state) {
  return state.ammo <= 0
}

// only these three names leave the file; MAX_AMMO would be private without this
return {
  MAX_AMMO
  reload
  isEmpty
}
```

```nut main.nut
let weapons = require("weapons.nut")

let rifle = { ammo = 0 }
println(weapons.isEmpty(rifle))          // true
println(weapons.reload(rifle).ammo)      // 240
```

`require_optional` differs only in what a missing file does:

```nut
let saveApi = require_optional("mods/customSaveFormat.nut")
if (saveApi)
  saveApi.write(currentSlot)
```

The two files above are an illustration, not a runnable example: the test runner
executes each committed sample alone, from a directory it controls, so a sample
cannot `require` another committed file. That is why every sample on this page that
does run reaches for a native module (`math`, `string`, and so on) instead of a file
of its own.

## import and from ... import

`import` and `from ... import` do at compile time what `require` does at run time, and
only work as the first statements in a file: write every one of them before any other
code. Once the compiler has moved past that opening run of imports, a later `import`
line no longer parses as one - it is read as a plain expression starting with the
identifier `import`, and fails to compile with `end of statement expected`.

`import "mod"` binds the whole exports object under the name `mod` itself;
`import "mod" as Name` binds it under `Name` instead.
`from "mod" import a, b as c` binds the individual exports `a` and `b` (renamed `c`),
and `from "mod" import *` binds every exported name at once.
Every bound name is a compile-time constant in the file that
imported it - not a variable - so a plain member access resolves without a lookup at
run time, and a pure, constant export can fold into the call site.

`import *` on the same `weapons.nut` as above, which binds each export under its own
name instead of behind the module object:

```nut main.nut
from "weapons.nut" import *

let rifle = { ammo = 0 }
println(isEmpty(rifle))                  // true
println(reload(rifle).ammo)              // 240
println(MAX_AMMO)                        // 240
```

Prefer the explicit `from "mod" import a, b` for maintainability.

{{example:language/modules-imports}}

## persist and keepref

A module's top level runs again on a hot reload - but the file's
persisted state should not reset just because its code was rewritten and reloaded.
`persist(key, initializer)` runs `initializer` once and remembers the result under
`key` (a string, unique within the file); every later run of the same `persist` call,
including one after a reload, returns that same remembered value instead of calling
`initializer` again. `key` only has to be unique among the `persist` calls in this
file, not across the whole project.

The remembered value must be something that can be mutated in place - a table, an
array, a class, an instance or a userdata - because the entire point of `persist` is
to keep the same container alive and change what is inside it. Passing an initializer
that returns a plain number, string or bool throws, since there is no way to update it
from outside without a new `persist` call replacing it, which would defeat the purpose.

`keepref(value)` roots `value` for the lifetime of the module manager, then returns it
unchanged - useful for an object that a module builds and installs into some engine
callback without keeping its own reference, which would otherwise leave nothing
holding it alive.

{{example:language/modules-persist}}

## Reloading

`require` is not part of the VM. It comes from `SqModules`, a library the host
builds on top of it, which is why a host may leave it out and why these functions do
not appear in this site's symbol reference: they are bound per module rather than
registered in a table the VM can enumerate.

That library is also what makes reloading possible. A reload does not re-run one
file: it drops every loaded module and executes the entry point again, so everything
reachable through `require` runs a second time with fresh code. Values kept by
[persist](page:language/modules#persist-and-keepref) survive that, which is the whole reason it exists -
a reload should replace behaviour without resetting the state the behaviour is
managing.

## Native modules

A native module is a table the host registers under a name, after which script
`require`s it exactly like a file. The standard library modules on this site are all
of that kind.

The host declares each function with a declaration string and an optional docstring:

```cpp
{ debug_setdebughook, "setdebughook(hook: function|null)",
  SQ_DOC("Installs the given function as the VM debug hook; null clears it") },
```

That single line is where the signature and the summary at the top of every symbol
page on this site come from - the VM reports them back through
[debug.get_function_decl_string](sym:debug.get_function_decl_string) and
[debug.doc](sym:debug.doc). A binding registered without a declaration string still
works, but the VM can then only report placeholder parameter names, which is why some
pages here carry a note saying so. `SQ_DOC` compiles the text away entirely when the
build sets `SQ_STORE_DOC_OBJECTS` to 0, so shipping builds need not carry it.

## __name__

`__name__` is the running file's own module name: `"__main__"` for the entry point
script, or whatever name it was `require`d under otherwise.

## See also

[modules.get_native_module_names](sym:modules.get_native_module_names) lists every
native module a given host has registered - useful for telling which optional native
modules (`io`, `datetime`, `async`, ...) a particular embedding actually shipped.
[modules.on_module_unload](sym:modules.on_module_unload) and
[modules.reset_static_memos](sym:modules.reset_static_memos) round out the module
system's own native module, `require("modules")`.
