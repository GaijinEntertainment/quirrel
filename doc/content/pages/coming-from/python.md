---
title: Coming from Python
group: Introduction
order: 1
summary: Braces instead of indentation, `//` is a comment, an empty container is true.
layout: mapping
---

Most of Python carries over: dynamic types, closures, generators, exceptions,
iteration over a container, and classes whose methods receive the instance. The
syntax changes. Blocks use braces, indentation has no meaning, a statement ends
at the end of the line, and every name is declared before it is read.

Two differences change what a program does without an error message: `//` is a
comment, and `""`, `[]` and `{}` are true. Read the traps even if you skip the
tables.

## Values and bindings

| Python | Quirrel |
| --- | --- |
| `x = 1`, rebindable | `local x = 1` - rebindable; `let x = 1` - binds once | page:language/bindings#let-and-local |
| `MAX = 10` by convention | `const MAX = 10 * 5`, folded at compile time | page:language/bindings#const-and-global-const |
| `None`, `True`, `False` | `null`, `true`, `false` | page:language/lexical#true-false-null |
| int and float are separate types | the same two, `integer` and `float` |
| `7 // 2` | `7 / 2`; two integers divide as integers | page:language/types#integers-and-floats |
| `7 / 2` | `7 / 2.0`; one float makes the answer a float | page:language/types#integers-and-floats |
| `2 ** 3` | [`pow(2, 3)`](sym:pow) from [math](sym:math) |
| `round(2.5)` gives `2` | `round(2.5)` gives `3`; a half goes away from zero | sym:round |
| `x if c else y` | `c ? x : y` | page:language/operators#all-operators |
| `f"{n} left"` | `$"{n} left"` | page:language/strings#interpolated-strings |
| `"a" + b` | [`$"a{b}"`](page:language/strings#interpolated-strings), or [concat](sym:types.String.concat); `+` joins too, and the analyzer flags it as `w264` |
| `"""two lines"""` | `@"two lines"`, a verbatim string with no escapes | page:language/strings#verbatim-strings |
| `str(x)`, `int(s)`, `float(s)` | `x.tostring()`, `s.tointeger()`, `s.tofloat()` | page:language/types#integers-and-floats |
| `len(s)` | `s.len()` | sym:types.String.len |
| `type(x).__name__` | `type(x)`, or `typeof x` | sym:type |
| `x is None` | `x == null` | page:language/operators#comparison-and-three-way-compare |
| `a and b`, `a or b`, `not a` | `a && b`, `a \|\| b`, `!a`, and they keep the operand value | page:language/operators#logical-operators |
| `1_000_000` | `1_000_000` | page:language/lexical#numbers |
| `0b1010` | no binary literal; only decimal `123` or hex `0xF2` |
| `# comment` | `// comment` or `/* ... */` | page:language/lexical#comments |

## Containers

| Python | Quirrel |
| --- | --- |
| `[1, 2, 3]` list | `[1, 2, 3]` array | page:language/containers#array-literals |
| `{"a": 1}` dict | `{ a = 1 }`, a table, with `=` inside | page:language/containers#table-literals |
| a key that is not a name | `{ [123] = 1 }`, any expression in the brackets | page:language/containers#table-literals |
| `d["k"] = v` on a new key | `d.k <- v`; plain `=` writes a slot that is already there | page:language/operators#the-newslot-operator |
| `d["k"]` on a missing key | it throws, as in Python; `d?[k]` gives `null`, and `??` fills in |
| `d.get(k, 0)` | `d?[k] ?? 0` | page:language/operators#null-coalescing-and-null-safe-access |
| `del d[k]` | `d.$rawdelete(k)` | sym:types.Table.rawdelete |
| `k in d` | `k in d` | page:language/operators#in-instanceof-typeof |
| `v in items` | `items.contains(v)`; `in` on an array asks about an index | sym:types.Array.contains |
| `d.keys()`, `d.values()`, `d.items()` | `d.keys()`, `d.values()`, `d.topairs()` | sym:types.Table.keys |
| `items.append(v)` | `items.append(v)`, and it takes several values | sym:types.Array.append |
| `a + b` for lists | `a.extend(b)`; `+` does not join two arrays | sym:types.Array.extend |
| `a[1:3]`, `a[-2:]` | `a.slice(1, 3)`, `a.slice(-2)` | sym:types.Array.slice |
| `s[0]` | `s.slice(0, 1)`; `s[0]` is the character code | sym:types.String.slice |
| `",".join(parts)` | `",".join(parts)` | sym:types.String.join |
| `s.split(",")`, `s.strip()` | `s.split(",")`, `s.strip()` | sym:types.String.split |
| `s.replace(a, b)` | `s.replace(a, b)`, and it replaces every match | sym:types.String.replace |
| `s.find(sub)` | `s.indexof(sub)`, and `null` when the text is not there | sym:types.String.indexof |
| `sorted(a)` | `let b = clone a` then `b.sort()`; `a.sort()` sorts in place | sym:types.Array.sort |
| `[f(x) for x in a if p(x)]` | `a.filter(@(v) p(v)).map(@(v) f(v))` | sym:types.Array.filter |
| `{**a, **b}` | `{ ...a, ...b }`, or `a.__merge(b)`; both return a new table | page:language/containers#spread |
| `copy.copy(a)` | `clone a`, one level deep | page:language/operators#clone |

## Control flow

| Python | Quirrel |
| --- | --- |
| `if a: elif b: else:` | `if (a) { } else if (b) { } else { }` | page:language/control-flow#if-else |
| `for v in seq:` | `foreach (v in seq)` | page:language/control-flow#foreach |
| `for i, v in enumerate(seq):` | `foreach (i, v in seq)` | page:language/control-flow#foreach |
| `for k, v in d.items():` | `foreach (k, v in d)` | page:language/control-flow#foreach |
| `for i in range(n):` | `for (local i = 0; i < n; i++)`; there is no `range` | page:language/control-flow#for |
| `while c:` | `while (c) { }` | page:language/control-flow#while-and-do-while |
| `break`, `continue` | the same |
| `match` | [`if (a) ... else if (b) ...`](page:language/control-flow#if-else); `switch` is deprecated and off by default, see [control flow](page:language/control-flow) |
| `try / except E as e / finally` | `try { } catch (e) { }`; no exception classes, and no `finally` | page:language/errors#throw-and-catch |
| `raise ValueError(m)` | `throw m`; any value can be thrown, and `catch` takes them all | page:language/errors#throw-and-catch |
| `assert c, m` | `assert(c, m)` | sym:assert |
| `with open(p) as f:` | no `with`; close the file yourself |
| `yield v` | `yield v` | page:language/generators#becoming-a-generator |
| `next(it)` | `resume it` | page:language/generators#states-and-resuming |
| `pass` | `{ }` |

## Functions

| Python | Quirrel |
| --- | --- |
| `def f(a, b = 1):` | `function f(a, b = 1) { }` | page:language/functions#declaring-one |
| `lambda x: x * 2` | `@(x) x * 2` | page:language/functions#lambdas |
| `def f(*args):` | `function f(...) { }`; the extra values arrive in `vargv` | page:language/functions#declaring-one |
| `def f(**kw):` | not there; take a table |
| `f(b = 1)` | not there: `'=' inside 'function argument' is forbidden` |
| `f(*args)` | `f.acall([this, a, b])` | sym:types.Function.acall |
| a nested `def` that reads an outer name | the same |
| `global x` | there are no implicit globals; `global` gives a `const` or `enum` to other files |
| a decorator | not there; wrap the function yourself |

## Classes

| Python | Quirrel |
| --- | --- |
| `class Squad:` | `class Squad { }` | page:language/classes#declaring-a-class |
| `class Squad(Unit):` | `class Squad(Unit) { }`, with one base class only | page:language/classes#declaring-a-class |
| `def __init__(self, n):` | `constructor(n) { }` | page:language/classes#declaring-a-class |
| `self` | `this`, and one method calls another as `this.other()` | page:language/functions#this |
| `super().__init__()` | `base.constructor()` | page:language/classes#inheritance-and-base |
| `self.hp = 100` in `__init__` | declare `hp = 100` in the class body; an instance takes no new slots later |
| `__str__` | `_tostring` | page:language/metamethods#type-name-and-text |
| `__getitem__`, `__call__` | [`_get`](page:language/metamethods#reading-and-writing-missing-slots), `_call`; see [metamethods](page:language/metamethods) |
| `@property` | `_get` and `_set` | page:language/metamethods#reading-and-writing-missing-slots |
| `isinstance(x, C)` | `x instanceof C` | page:language/classes#instanceof |
| a class attribute | `static max = 3`, one value for every instance | page:language/classes#static-members |

## Modules

| Python | Quirrel |
| --- | --- |
| `import math` | `let math = require("math")`, or `import "math"` at compile time | page:language/modules#require-and-require-optional |
| `from math import floor` | `from "math" import floor` | page:language/modules#import-and-from-import |
| `from math import *` | `from "math" import *` | page:language/modules#import-and-from-import |
| `import numpy as np` | `import "mod" as np` | page:language/modules#import-and-from-import |
| a module runs once and is cached | the same, and [modules](page:language/modules) says when |

## Traps

- **`//` is a comment.** `let half = total // 2` keeps `total` and drops the rest
  of the line. There is no floor division operator, because `/` on two integers
  already truncates.
- **It truncates, it does not floor.** `-7 / 2` is `-3` where Python gives `-4`, and
  `-7 % 3` is `-1` where Python gives `2`. The sign follows the left side, as in C.
- **Only `null`, `false`, `0` and `0.0` are false.** `""`, `[]` and `{}` are all
  true, so `if (!items)` never fires. Test `items.len() == 0`.
- **`in` tests for a key.** The keys of an array are its indices, so
  `2 in [10, 20, 30]` is true and `99 in [10, 20, 30]` is false. Use `contains` to
  search for a value.
- **`s[0]` is a number.** Indexing a string gives the character code, `97` for
  `"a"`. There is no character type.
- **`clone` is an operator, not a function.** `clone(a).append(9)` parses as
  `clone (a.append(9))`, which appends to `a`. Write `(clone a).append(9)`.
- **A container in a class body is one object for every instance**, like a
  mutable class attribute in Python. Declare the slot as `null` and fill it in the
  constructor.
- **A default parameter value is built once**, so a mutable default is shared
  between calls. Python has the same trap, for the same reason.
- **`1 / 0` throws.** Arithmetic never gives `inf` or a quiet `nan`.
- **`1 < x < 3` is not a chained comparison.** It compares `(1 < x)` with `3`, and
  a bool against a number throws.

{{example:coming-from/python}}

## What is not there

- tuples, sets, comprehensions, `with`, decorators, keyword arguments
- exception classes: `catch (e)` receives the thrown value
- `finally`, multiple inheritance, `**`, `//`
- big integers: an `integer` is 64 bits and it wraps
- a large standard library: the modules are [math](sym:math),
  [string](sym:string), [io](sym:io), [iostream](sym:iostream),
  [datetime](sym:datetime), [debug](sym:debug), `system` and [async](sym:async)

## See also

- [Cheat sheet](page:cheatsheet) - the language on two printable pages
- [Traps](page:cheatsheet#traps) - traps for every user, not only Python users
- [Values and types](page:language/types) and [Tables and arrays](page:language/containers)
- [Bindings and constants](page:language/bindings) - `let`, `local`, `const`, `global`
- Coming from [JavaScript](page:coming-from/javascript),
  [Lua](page:coming-from/lua), [Squirrel](page:coming-from/squirrel)
