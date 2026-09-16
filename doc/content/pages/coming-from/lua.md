---
title: Coming from Lua
group: Introduction
order: 3
summary: Indices start at 0, an array is not a table, and `0` is false.
layout: mapping
---

Both languages are small, embedded, dynamically typed and reference counted, and
both are made to be driven from C. The differences are around tables: an array
and a table are separate types, indices start at 0, a missing key is an error
rather than `nil`, and no name is global unless you declare it global.

The syntax is the C family: braces, `!=`, `&&`, `//` for a comment.

## Values and bindings

| Lua | Quirrel |
| --- | --- |
| `local x = 1` | `local x = 1` - rebindable; `let x = 1` - binds once | page:language/bindings#let-and-local |
| a name with no `local` is global | there are no implicit globals; every name is declared |
| `nil` | `null` | page:language/lexical#true-false-null |
| `x == nil` | `x == null` | page:language/operators#comparison-and-three-way-compare |
| `~=` | `!=` | page:language/operators#comparison-and-three-way-compare |
| `..` | [`$"{a}{b}"`](page:language/strings#interpolated-strings), or [concat](sym:types.String.concat); `+` joins too, and the analyzer flags it as `w264` |
| `#s`, `#t` | `s.len()`, `t.len()` | sym:types.String.len |
| `and`, `or`, `not` | `&&`, `\|\|`, `!`, and they keep the operand value | page:language/operators#logical-operators |
| one number type | `integer` and `float`; `7 / 2` is `3`, `7 / 2.0` is `3.5` | page:language/types#integers-and-floats |
| `//` floor division | `/` on two integers, which truncates; `//` starts a comment | page:language/operators#arithmetic |
| `-7 % 3` is `2` | `-1`; the sign follows the left side, as in C | page:language/operators#arithmetic |
| `tostring(x)`, `tonumber(s)` | `x.tostring()`, `s.tointeger()`, `s.tofloat()` | page:language/types#integers-and-floats |
| `string.format(...)` | [`format(...)`](sym:format) from [string](sym:string) |
| `[[two lines]]` | `@"two lines"`, a verbatim string with no escapes | page:language/strings#verbatim-strings |
| `-- comment` | `// comment` or `/* ... */` | page:language/lexical#comments |

## Tables and arrays

| Lua | Quirrel |
| --- | --- |
| `{ 1, 2, 3 }`, keys 1 to 3 | `[1, 2, 3]`, an array, and its first index is 0 | page:language/containers#array-literals |
| `{ a = 1 }` | `{ a = 1 }`, a table, with `=` inside | page:language/containers#table-literals |
| `t.k = v` on a new key | `t.k <- v`; plain `=` writes a slot that is already there | page:language/operators#the-newslot-operator |
| `t.k` on a missing key gives `nil` | it throws; `t?.k` gives `null`, and `??` fills in |
| `t[k] = nil` removes the key | it stores `null` and keeps the slot; `t.$rawdelete(k)` removes |
| `#a` | `a.len()` | sym:types.Array.len |
| `table.insert(a, v)` | `a.append(v)`, and it takes several values | sym:types.Array.append |
| `table.insert(a, 1, v)` | `a.insert(0, v)` | sym:types.Array.insert |
| `table.remove(a, i)` | `a.remove(i)` | sym:types.Array.remove |
| `table.remove(a)` | `a.pop()` | sym:types.Array.pop |
| `table.concat(a, ",")` | `",".join(a)`, which is a string method | sym:types.String.join |
| `table.sort(a, cmp)` | `a.sort(cmp)`, with `@(x, y) x <=> y` | sym:types.Array.sort |
| `pairs(t)` | `foreach (k, v in t)` | page:language/control-flow#foreach |
| `ipairs(a)` | `foreach (v in a)`, or `foreach (i, v in a)` for the index too | page:language/control-flow#foreach |
| `next(t)` | not there; use `foreach` |
| `s:sub(2, 3)`, 1-based and inclusive | `s.slice(1, 3)`, 0-based and end exclusive | sym:types.String.slice |
| `s:find(sub)` | `s.indexof(sub)`, and `null` when the text is not there | sym:types.String.indexof |
| `s:gsub(a, b)` for plain text | `s.replace(a, b)`, and it replaces every match | sym:types.String.replace |
| a Lua pattern | [`regexp("[0-9]+")`](sym:regexp) from [string](sym:string) |
| `setmetatable(t, mt)` | not there; a class carries the metamethods |
| `mt.__index` | a base class, or the `_get` [metamethod](page:language/metamethods) |
| `mt.__add`, `mt.__tostring` | `_add`, `_tostring`, on a class | page:language/metamethods#arithmetic |

## Control flow and functions

| Lua | Quirrel |
| --- | --- |
| `if a then ... elseif b ... end` | `if (a) { } else if (b) { } else { }` | page:language/control-flow#if-else |
| `for i = 1, n do` | `for (local i = 0; i < n; i++)` | page:language/control-flow#for |
| `while c do ... end` | `while (c) { }` | page:language/control-flow#while-and-do-while |
| `repeat ... until c` | `do { } while (!c)` | page:language/control-flow#while-and-do-while |
| `goto` | not there |
| `function f(a) end` | `function f(a) { }` | page:language/functions#declaring-one |
| `local function f()` | `function f()` is already local to the file | page:language/functions#declaring-one |
| `function t.m(a)` | `let t = { function m(a) { } }` | page:language/containers#table-literals |
| `function t:m(a)` and `self` | `function m(a)` in a table or class, and `this` | page:language/functions#declaring-one |
| `t:m(x)` | `t.m(x)` | page:language/functions#this |
| `return a, b` | return an array: `return [a, b]`, read as `let [a, b] = f()` |
| `...` and `select("#", ...)` | `function f(...)` and `vargv.len()` | page:language/functions#declaring-one |
| `f{ ... }` and `f"..."` | not there; a call always has parentheses |
| `pcall(f)` | `try { f() } catch (e) { }`; a `pcall` here is a call that skips the error handler, and it does not catch | page:language/errors#throw-and-catch |
| `error("m")` | `throw "m"`; any value can be thrown, and `catch` takes them all | page:language/errors#throw-and-catch |
| `coroutine.create(f)` | `newthread(f)` | sym:newthread |
| `coroutine.resume(co, v)` | `th.call(v)` the first time, `th.wakeup(v)` after that | sym:types.Thread.call |
| `coroutine.yield(v)` | `suspend(v)` in a thread, `yield v` in a generator | sym:suspend |
| `require "m"` | `require("m")`, or `import "m"` at compile time | page:language/modules#require-and-require-optional |
| `os.clock()` | [`clock()`](sym:clock) from [datetime](sym:datetime) |
| `io.write(s)` | `print(s)`, or `println(s)` with a newline | sym:print |
| `os.getenv(n)` | [getenv](sym:system.getenv), in the [system](sym:system) module |

## Traps

- **Indices start at 0**, and the second argument of `slice` is one past the end.
  Every loop bound and every `sub` call must move by one.
- **`0` is false.** Lua counts only `nil` and `false` as false. In Quirrel `0` and
  `0.0` are false too, so `if (count)` is a different test. `""`, `[]` and `{}`
  stay true.
- **`+` does not read a number out of a string.** `"10" + 1` is `"101"`, not `11`.
  Convert with `.tointeger()`. Every other arithmetic operator throws on a string.
- **A stored `null` is not a removed key.** `t.k = null` leaves the slot, and
  `"k" in t` is still true. Use `t.$rawdelete("k")`.
- **A missing key throws.** There is no `nil` to test afterwards, so test first with
  `in`, or read with `t?.k ?? dflt`.
- **`%` follows C.** `-7 % 3` is `-1` here and `2` in Lua.
- **`//` is a comment**, so a Lua 5.3 floor division drops the rest of the line
  with no error.
- **One return value only.** A function returns one value. Pack the rest into an
  array or a table and destructure it.
- **A method accesses its own object through `this`.** There is no implicit `self`
  lookup. Inside a method, call another method as `this.other()`.
- **Table order is undefined**, and the hash seed changes between runs. Sort the
  keys when the order is visible.

{{example:coming-from/lua}}

## What is not there

- metatables on a plain table: `setmetatable` and `setdelegate` are both gone
- multiple return values, `goto`, `repeat ... until`, Lua string patterns
- implicit globals, and the root table is deprecated
- integer keys and array parts in one type; an array is a separate type
- `#` as an operator, `..` as an operator, `elseif`, `then`, `end`, `do`

## See also

- [Cheat sheet](page:cheatsheet) - the language on two printable pages
- [Traps](page:cheatsheet#traps) - traps for every user, not only Lua users
- [Tables and arrays](page:language/containers) and [Values and types](page:language/types)
- [Generators and threads](page:language/generators) - the Quirrel form of coroutines
- Coming from [Python](page:coming-from/python),
  [JavaScript](page:coming-from/javascript), [Squirrel](page:coming-from/squirrel)
