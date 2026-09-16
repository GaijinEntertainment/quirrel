---
title: Coming from JavaScript
group: Introduction
order: 2
summary: No `undefined`, two number types, and a missing key throws.
layout: mapping
---

The two languages look alike, so the differences are easy to miss. Braces,
`for`, `while`, `function`, closures, `class`, `try`/`catch`, `?.`, `??`, string
interpolation and `async`/`await` have the same meaning. The differences are
inside common expressions: there is no `undefined`, integers and floats are
separate types, and reading a key that is not there is an error.

## Values and bindings

| JavaScript | Quirrel |
| --- | --- |
| `let x = 1`, rebindable | `local x = 1` - rebindable; `let x = 1` - binds once | page:language/bindings#let-and-local |
| `const x = 1` | `let x = 1`; `const MAX = 10 * 5`, folded at compile time | page:language/bindings#let-and-local |
| `var` | nothing |
| `null` and `undefined` | `null` alone | page:language/lexical#true-false-null |
| one number type | `integer` and `float`; `7 / 2` is `3`, `7 / 2.0` is `3.5` | page:language/types#integers-and-floats |
| `x === y` | `x == y`; there is no loose equality to avoid | page:language/operators#comparison-and-three-way-compare |
| a template literal, `${b}` inside | `$"a {b}"` | page:language/strings#interpolated-strings |
| `(x) => x * 2` | `@(x) x * 2` | page:language/functions#lambdas |
| `function (x) { }` | the same |
| `typeof x` | `type(x)`, or `typeof x`, giving `"integer"`, `"table"`, `"instance"`, ... | sym:type |
| `x ?? y`, `x?.y`, `a?.[i]`, `f?.()` | `x ?? y`, `x?.y`, `a?[i]`, `f?()` | page:language/operators#null-coalescing-and-null-safe-access |
| `!x`, `&&`, `\|\|` | the same, and they keep the operand value |
| `x++`, `+=`, `<<`, `&` | the same |
| `NaN`, `Infinity` | neither one; `1 / 0` throws |
| `String(x)`, `Number(s)` | `x.tostring()`, `s.tointeger()`, `s.tofloat()` | page:language/types#integers-and-floats |
| `// comment` | the same |

## Objects and arrays

| JavaScript | Quirrel |
| --- | --- |
| `{ a: 1 }` | `{ a = 1 }`, a table, with `=` inside; `{ [expr] = 1 }` for a computed key | page:language/containers#table-literals |
| `{ x, y }` shorthand | `{ x, y }`, the same | page:language/containers#table-literals |
| `o.miss` gives `undefined` | it throws; `o?.miss` gives `null`, and `??` fills in |
| `o.fresh = v` | `o.fresh <- v`; plain `=` writes a slot that is already there | page:language/operators#the-newslot-operator |
| `delete o.k` | `o.$rawdelete("k")`; the `delete` operator is off by default | sym:types.Table.rawdelete |
| `"k" in o`, `o.hasOwnProperty(k)` | `"k" in o`, `o.rawin(k)` | page:language/operators#in-instanceof-typeof |
| `Object.keys`, `values`, `entries` | `o.keys()`, `o.values()`, `o.topairs()` | sym:types.Table.keys |
| `Object.assign({}, a, b)` | `{ ...a, ...b }`, or `a.__merge(b)`; `a.__update(b)` updates in place | sym:types.Table.__merge |
| `Object.freeze(o)` | `freeze(o)`, and it reaches the tables inside | sym:freeze |
| `{ ...o }`, `[...a]` | the same, one level deep; a table also takes a class or an instance, and a `null` source adds nothing | page:language/containers#spread |
| `{ ...a, ...b }`, `[...a, ...b]` | the same, and a later key wins | page:language/containers#spread |
| `structuredClone(o)` | nothing; `clone o` copies one level | page:language/operators#clone |
| `const { a, b } = o` | `let { a, b } = o`, and a key that is not there throws unless it has a default | page:language/destructuring |
| `const { a, ...rest } = o` | `let { a } = o`; there is no rest element | page:language/destructuring |
| `a.length` | `a.len()` | sym:types.Array.len |
| `a.push(v)`, `a.pop()` | `a.append(v)`, `a.pop()` | sym:types.Array.append |
| `a.shift()`, `a.unshift(v)` | `a.remove(0)`, `a.insert(0, v)` | sym:types.Array.remove |
| `a.indexOf(v)` | `a.indexof(v)`, and `null` rather than `-1` | sym:types.Array.indexof |
| `a.includes(v)` | `a.contains(v)` | sym:types.Array.contains |
| `a.forEach(f)` | `a.each(f)` | sym:types.Array.each |
| `a.map`, `a.filter`, `a.reduce` | the same names, and the callback is `(value, index)` |
| `a.find`, `a.findIndex` | `a.findvalue`, `a.findindex` | sym:types.Array.findvalue |
| `a.concat(b)` | `a.extend(b)`; `+` does not join two arrays | sym:types.Array.extend |
| `a.join(",")` | `",".join(a)`, which is a string method | sym:types.String.join |
| `a.slice(1, 3)`, `a.slice(-2)` | the same |
| `s.toUpperCase()`, `s.trim()` | `s.toupper()`, `s.strip()` | sym:types.String.toupper |
| `s.startsWith(p)`, `s.includes(p)` | `s.startswith(p)`, `s.contains(p)` | sym:types.String.startswith |
| `s.replace(a, b)` | `s.replace(a, b)`, and it replaces every match | sym:types.String.replace |
| `/re/` literal | [`regexp("re")`](sym:regexp) from [string](sym:string) |
| `JSON.parse`, `JSON.stringify` | not in the core library |
| `Map`, `Set` | a table |

## Control flow and functions

| JavaScript | Quirrel |
| --- | --- |
| `for (const v of a)` | `foreach (v in a)` | page:language/control-flow#foreach |
| `for (const k in o)` | `foreach (k, v in o)`, which gives both | page:language/control-flow#foreach |
| `for (let i = 0; ...)` | `for (local i = 0; ...)`; `let` is not allowed in a `for` header | page:language/control-flow#for |
| `switch` | [`if (a) ... else if (b) ...`](page:language/control-flow#if-else); `switch` is deprecated and off by default, see [control flow](page:language/control-flow) |
| `try / catch / finally` | `try { } catch (e) { }`; no exception classes, and no `finally` | page:language/errors#throw-and-catch |
| `throw new Error(m)` | `throw m`; any value can be thrown, and `catch` takes them all | page:language/errors#throw-and-catch |
| `async` and `await` | the same words; see [async and await](page:language/async) |
| `function* g()` with `yield` | any function that contains `yield` |
| `f(...args)` | `f.acall([this, a, b])` | sym:types.Function.acall |
| `f.call(o, x)`, `f.apply(o, args)` | `f.call(o, x)`, `f.acall([o, x])` | sym:types.Function.call |
| `f.bind(o)` | `f.bindenv(o)` | sym:types.Function.bindenv |
| `arguments` | `vargv`, in a `function f(...)` | page:language/functions#declaring-one |
| a default parameter | `function f(a, b = 1) { }`, the same | page:language/functions#declaring-one |

## Classes

| JavaScript | Quirrel |
| --- | --- |
| `class A extends B` | `class A(B) { }`, with one base class only | page:language/classes#declaring-a-class |
| `constructor()` | `constructor()` | page:language/classes#declaring-a-class |
| `super.m()`, `super()` | `base.m()`, `base.constructor()` | page:language/classes#inheritance-and-base |
| a field assigned in the constructor | declare it in the class body; an instance takes no new slots later |
| `static x = 1` | `static x = 1` | page:language/classes#static-members |
| `get x()`, `set x(v)` | the `_get` and `_set` [metamethods](page:language/metamethods) |
| `toString()` | `_tostring` | page:language/metamethods#type-name-and-text |
| `new A()` | `A()`; there is no `new` | page:language/classes#instantiation |
| `a instanceof A` | the same |
| a prototype patched at run time | a class is locked as soon as it is used |

## Modules

| JavaScript | Quirrel |
| --- | --- |
| `import { x } from "m"` | `from "m" import x` | page:language/modules#import-and-from-import |
| `import * as m from "m"` | `import "m" as m` | page:language/modules#import-and-from-import |
| `require("m")` | `require("m")`, at run time; `import "m"` at compile time | page:language/modules#require-and-require-optional |
| `export default v` | `return v` at the end of the file | page:language/modules |
| `export const a, b` | `return freeze({ a, b })` | page:language/modules |

## Traps

- **There is no `undefined`.** Reading a slot that is not there throws, and so does
  reading a member of `null`. `o?.x` gives `null`, and `??` fills it in.
- **A missing slot named like a type method is not `null`.** When `t` has no
  `filter` slot, `t?.filter` finds `Table.filter`. Test with `"filter" in t`, or
  write `t.$filter` when you want the type method.
- **`7 / 2` is `3`.** Two integers divide as integers. Make one side a float.
- **`1 / 0` throws** instead of giving `Infinity`, and `0.0 / 0.0` throws instead of
  giving `NaN`.
- **`""`, `[]` and `{}` are true.** `0` and `0.0` are false, as in JavaScript, but
  an empty string or container is not. Test `s.len() == 0`.
- **`for` gives every closure the same variable**, the way `var` did. `foreach`
  gives each pass its own. The sample below shows both.
- **`==` on two tables compares identity**, like `===` on two objects. There is no
  deep comparison in the language.
- **`+` joins when one side is a string** and throws for other mixed pairs, so
  `1 + true` throws. Use `$"..."`. The analyzer flags `+` on a string as `w264`.
- **A container in a class body is one object for every instance.** A JavaScript
  class field is built per instance; this one is not. Declare the slot as `null` and
  fill it in the constructor.
- **`clone` is an operator, not a function.** `clone(a).append(9)` parses as
  `clone (a.append(9))`, which appends to `a`. Write `(clone a).append(9)`.

{{example:coming-from/javascript}}

## What is not there

- `undefined`, `NaN`, `Infinity`, `finally`, `new`, labelled statements
- prototypes, `Symbol`, `Proxy`, `Map`, `Set`, `JSON`, `Promise` (the runtime has
  [Future](sym:async.Future) instead)
- spread in a call, a rest element in destructuring, tagged templates, a regular
  expression literal
- truthiness for an empty string or container
- automatic number growth: an `integer` is 64 bits and it wraps

## See also

- [Cheat sheet](page:cheatsheet) - the language on two printable pages
- [Traps](page:cheatsheet#traps) - traps for every user, not only JavaScript users
- [Null safety](page:language/operators) - `?.`, `?[`, `?()` and `??`
- [Classes and instances](page:language/classes) and [Modules](page:language/modules)
- Coming from [Python](page:coming-from/python), [Lua](page:coming-from/lua),
  [Squirrel](page:coming-from/squirrel)
