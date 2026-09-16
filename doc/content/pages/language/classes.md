---
title: Classes and instances
group: Language
order: 55
summary: Class bodies, instances, inheritance and static members.
---

A class is a value like any other: it can be stored in a variable, passed to
a function, and kept in a table or array. It describes the fields and
methods every instance made from it will have.

## Declaring a class

`class Name { ... }` declares fields with a default value and methods,
either as `function name() { ... }` or as a field assigned a function or a
lambda. `constructor` is a reserved method name, called automatically for
every new instance. Inside a method, a field is always reached through
`this.` - unlike some other object-oriented languages, a bare name does not
implicitly mean `this.name`.

{{example:language/classes-basics}}

## Inheritance and base

A derived class copies every field, static and method from its base first,
then applies its own body over that: `class Tank(Vehicle) { ... }`. `base`
reaches the shadowed implementation from inside an override, most often
`base.constructor(...)` or `base.someMethod(...)`. There is no `super`
keyword; writing `super` is just an unknown variable and fails to compile.

A derived class that declares no `constructor` of its own inherits the
base's, called with whatever arguments the instantiation call passes.
[`getbase()`](sym:types.Class.getbase) returns the class given in the
parentheses, or `null` for a class with none.

{{example:language/classes-inheritance}}

The keyword `extends` appears in the grammar (`class Tank extends Vehicle`)
but the lexer never produces it, so it is dead syntax that fails to parse
with `expected '{'`. The parenthesized form above is the one that works.

## Static members

`static name = value`, written inside the class body, is one value shared by
every instance rather than a per-instance default. A static is read-only
after declaration: neither `ClassName.name = value` nor `this.name = value`
from a method can change it - the first throws `trying to set 'class'`, the
second `the index 'name' (type='string') does not exist`, because a static
is not stored per instance at all. The only way to add or replace one after
the class already exists is [`newmember`](sym:types.Class.newmember) with
its third argument set to `true`.

## Instantiation

`ClassName(args)` creates an instance, copies the class's field defaults into
it, and runs `constructor` if one is declared. The defaults are copied
verbatim, not cloned: if a default is an array or a table and no constructor
replaces it, every instance that skips that field shares the very same
array. Give a mutable default its own value inside `constructor` when each
instance needs an independent one. [`instance()`](sym:types.Class.instance)
makes an instance the same way, without running `constructor` at all.

Every instance remembers the exact class it was made from -
[`getclass()`](sym:types.Instance.getclass) returns it, never a base class,
even for a class made with `class Tank(Vehicle) { ... }`.

## instanceof

`x instanceof ClassName` walks `x`'s class and its bases, and never throws
just because `x` is not an instance at all - `5 instanceof Squad` is a legal
`false`, not an error. It does throw when the right-hand side is not a class
value at all: `squad instanceof 5` throws `cannot apply instanceof between a
integer and a instance` (the message names the right operand's type first,
then the left's). An instance's `instanceof` against the built-in placeholder
[`Instance`](sym:types.Instance) class from the `types` module is always
`false`, whatever class made it; see [Values and types](page:language/types)
for why, and for the `type(x) == "instance"` check that does work for "is
this any instance at all".

## Locking

A class permanently locks the first time it gains an instance, becomes the
base of another class, or has [`lock()`](sym:types.Class.lock) called on it
directly - whichever happens first. Locking stops a new plain field from
ever being added: `ClassName.newField <- value` then throws `trying to
modify a class that has already been instantiated, inherited or is locked
manually`. It does not stop a method being added or replaced the same way,
since `<-` with a function value bypasses the lock check; nor does it stop
[`newmember`](sym:types.Class.newmember) from adding a new **static**, when
its third argument asks for one.

## Metamethods

A class can define any of the seventeen metamethods the VM recognizes -
`_add _sub _mul _div _unm _modulo`, `_set _get _newslot _delslot`, `_typeof
_cmp _nexti _call _cloned _tostring`, and `_lock` - as an ordinary method
named that way. [`getmetamethod`](sym:types.Class.getmetamethod) looks one
up by name without calling it. All of them run with an instance as `this`
except `_lock`, which runs once, at the moment the class itself locks, with
the class object as `this` - it is the one metamethod a class can never see
fired on an instance.

[Metamethods](page:language/metamethods) gives each one its own contract:
when the VM calls it, what it receives, and what it must return or throw.

{{example:language/classes-metamethods}}

## Edge cases

- A class body is not quite table syntax: a quoted-string key (`"key":
  value`), the bare-name shorthand (`{ name }` for `name = name`) and a
  [spread](page:language/containers#spread) (`...src`) all parse in a
  [table](page:language/containers) literal but fail to compile in a class
  body, which requires `identifier = value` for every field.
- `clone` works on an instance (running `_cloned` if the class defines it)
  but never on the class object itself: `clone SomeClass` throws `cloning a
  class`.
- `_newslot` lets a class intercept `<-` on its own instances - normally
  that throws `class instances do not support the new slot operator` - but
  the instance still cannot literally gain the slot: an instance's storage
  is a fixed-size block sized at construction, so even `this.rawset(key,
  val)` called from inside the metamethod throws `the index '<key>'
  (type='<type>') does not exist`. Store the extra data somewhere else, such
  as a side table, rather than trying to complete the slot.
