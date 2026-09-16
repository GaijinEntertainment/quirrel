---
title: Sqrat bindings
group: Embedding
order: 114
summary: Sqrat, the C++ binding layer over the raw API.
---

Sqrat is the C++ layer over the raw API. It converts between C++ and Quirrel
types on its own, so a function or a class can be exposed without stack code for
each one. Most hosts use it and touch `squirrel.h` only for what Sqrat does not
cover.

```cpp
#include <sqrat.h>
#include <sqModules.h>
```

## A module

`SqModules` publishes a table as something a script can `require` or `import`.
Building that table is the registration step:

```cpp
Sqrat::Table exports(vm);
exports
  .Func("area", &compute_area)
  .SquirrelFuncDeclString(do_math, "pure do_math(a: int, [b: number = 2]): float",
                          SQ_DOC("Performs the math operation. b defaults to 2."))
  .SetValue("MAX_SQUADS", 12);

module_manager->addNativeModule("geometry", exports);
```

Script side:

```nut
from "geometry" import area, MAX_SQUADS
```

## Binding a function

`Func` takes a C++ function or method pointer and works out the conversions:

```cpp
exports.Func("pow", &std::pow);
```

`SquirrelFuncDeclString` is the form to prefer for anything more involved. The
declaration string carries the signature, the optional arguments and their
defaults, the return type and the attributes in one place. With it the VM can
type-check the call, name the parameters in an error, fold a `pure` call at
compile time, and report the function to tools. Every signature on this site
comes from a declaration string.

`SquirrelFunc` is the older, low-level form. It gives you the raw VM and a type
mask such as `"tsn|p"`, and you read the stack yourself. It is deprecated for
new bindings: a type mask cannot name a parameter, and nothing checks that the
mask matches what the body reads. Where you still meet one, the mask letters
are `o` null, `i` integer, `f` float, `n` number, `s` string, `t` table, `a`
array, `u` userdata, `c` closure, `g` generator, `p` userpointer, `v` thread,
`x` instance, `y` class, `b` bool, `r` weakref and `.` for anything, with `|`
between alternatives.

## Binding a class

```cpp
Sqrat::Class<Rect> rectClass(table.GetVM(), "Rect");
rectClass
  .Ctor()
  .Var("width", &Rect::width)
  .Var("height", &Rect::height)
  .Func("area", &Rect::area)
  .Prop("perimeter", &Rect::perimeter);

exports.Bind("Rect", rectClass);
```

`Var` exposes a field a script can read and write. `Prop` exposes a getter as a
field, so `r.perimeter` reads without parentheses. `Func` exposes a method.

`SquirrelCtor` replaces `Ctor` when construction needs logic - overloads, a copy
constructor, validation. It receives the raw VM, builds the C++ object itself,
and links it to the script instance with
`Sqrat::ClassType<T>::SetManagedInstance`. Without that last call the script has
an instance with nothing behind it.

## Where to look next

This page is an introduction. It does not cover reading a script value from
C++, const tables, property setters or static members. The samples and the
Sqrat headers are the reference for those. For the layer underneath, see
[Native functions](page:embedding/natives).
