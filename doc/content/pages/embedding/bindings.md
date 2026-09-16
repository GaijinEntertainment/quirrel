---
title: Sqrat bindings
group: Embedding
order: 114
summary: Sqrat, the C++ layer that writes the stack code for you.
---

Sqrat is the C++ layer over the raw API. It converts between C++ and Quirrel types
on its own, so exposing a function or a class does not mean writing stack code for
each one. Most hosts use it and touch `squirrel.h` only where Sqrat has no answer.

```cpp
#include <sqrat.h>
#include <sqModules.h>
```

## A module

`SqModules` publishes a table as something a script can `require` or `import`.
Building that table is the whole registration step:

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
declaration string carries the signature, the optional arguments and their defaults,
the return type and the attributes, all in one place, and that is what lets the VM
type-check the call, name the parameters in an error, fold a `pure` call at compile
time, and report the function to tools. Every signature on this site comes from one.

`SquirrelFunc` is the older, low-level form: it hands you the raw VM and a type mask
such as `"tsn|p"`, and you read the stack yourself. It is deprecated for new
bindings, because a type mask cannot name a parameter and nothing validates that the
mask matches what the body actually reads. Where you still meet one, the mask letters
are `o` null, `i` integer, `f` float, `n` number, `s` string, `t` table, `a` array,
`u` userdata, `c` closure, `g` generator, `p` userpointer, `v` thread, `x` instance,
`y` class, `b` bool, `r` weakref and `.` for anything, with `|` between alternatives.

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

`Var` exposes a field a script can read and write. `Prop` exposes a getter as
something that looks like a field, so `r.perimeter` reads without parentheses.
`Func` exposes a method.

`SquirrelCtor` replaces `Ctor` when construction needs real logic - overloads, a
copy constructor, validation. It receives the raw VM, builds the C++ object itself,
and links it to the script instance with
`Sqrat::ClassType<T>::SetManagedInstance`. Miss that last call and the script has an
instance with nothing behind it.

## Where to look next

This is an introduction, not the whole library: consuming a script value from C++,
const tables, property setters and static members are all beyond it. The samples and
the Sqrat headers are the reference. For the layer underneath, see
[Native functions](page:embedding/natives).
