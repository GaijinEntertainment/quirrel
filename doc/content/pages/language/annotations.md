---
title: Type annotations
group: Language
order: 80
summary: Type annotations on a parameter, a return type or a binding.
---

A name may carry `: Type` after it - a parameter, a return type after `):`,
a `local` or `let` declaration, a destructured field or element, or the
vararg tail. This is core syntax, parsed unconditionally; there is no flag
that turns it on or off in a script.

## Syntax

The fourteen types from [Values and types](page:language/types) all work as
annotations, plus two that exist only there: `number`, shorthand for
`int|float`, and `any`, which accepts every value and opts a parameter out of
checking. Combine types with `|`; parentheses may group a union for clarity.
A default value still works together with a type, most usefully to spell an
optional nullable parameter as `hp: int|null = null`.

{{example:language/annotations-syntax}}

## What the compiler does

An annotation becomes a check at the point it guards: a parameter is checked
when its function is entered, a return value when the function returns, a
declared or assigned variable when the write happens, and a destructured
field or element when the destructuring runs.

When the incoming value's type is not known until the check runs - a
parameter, most assignments - the check is a runtime one, and a mismatch
throws. When it is provable from the expression alone - assigning a literal
directly - the compiler rejects it up front instead:

```nut
function badReturn(): int {
  return "not an int"  // error: expression of type 'string' cannot be
}                       // assigned to type 'int', caught at compile time
```

{{example:language/annotations-runtime-check}}

## What it does not do

An annotation is a gate, not a conversion: it never coerces the value to
match. `x: number` accepts an int or a float exactly as given, so a function
that only ever receives ints still returns an int. Quirrel has no function
overloading, so an annotation cannot select between two bodies by argument
type either - there is exactly one body, whatever the declared types say.
And code that is never exercised with a wrong-typed value never trips the
check, so an annotation is not proof that every caller agrees with it, only
that every caller so far has.

{{example:language/annotations-no-coercion}}

## Declaration strings

A native function has no Quirrel source to annotate, so its binding carries
the same syntax as a string instead - `pure type(obj): string`,
`getbuildinfo(): table` - one declaration string per native, which is what
every signature box on this site is rendered from. `sq --parse-types
somefile.txt` parses a file of such strings, one per line, and prints what it
understood, which is how the string grammar itself gets tested:

```text
sq --parse-types decls.txt   # decls.txt holds one declaration per line

pure clampAmmo(current: int, maxAmmo: int): int

pure clampAmmo(current: int, maxAmmo: int): int
  functionName: clampAmmo
  returnTypeMask: 0x2
  objectTypeMask: 0xffffffff
  ellipsisArgTypeMask: 0x0
  requiredArgs: 2
  argCount: 2
  pure: true
  nodiscard: false
```

`--parse-types` is a tool for that string grammar, not a way to enable
annotations in ordinary scripts - those are always parsed, flag or not.
