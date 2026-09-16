---
params: [other]
see_also: [types.Table.__merge, types.Table.replace_with, types.Table.rawset]
---

Copies every slot from each argument into the table, in place, later
arguments overriding earlier ones.

## Parameters

- `other` - a table, class or instance to copy slots from; repeatable, so
  more than one may be given

## Return value

The table itself.

## Errors

Throws `Cannot modify immutable object` when the table was frozen with
`freeze()`.

Throws a type error when an argument is not a table, class or instance, but
the wording depends on which one: a bad first `other` throws `parameter 1 of
'__update' has an invalid type '...' ; expected: 'table|class|instance'`,
while a bad second or later argument throws `parameter N of '<unknown>' has
an invalid type '...' ; expected: 'table|class|instance'`, with `N` counting
the table itself as parameter 1. Only the first argument goes through the
VM's own automatic type check, which knows the function's name; the rest are
checked by hand inside `__update`, which does not pass a name along.

## Notes

Takes 1 or more arguments - this method is variadic, unlike
[`reduce`](sym:types.Table.reduce) or [`findvalue`](sym:types.Table.findvalue)
on this page, where a negative arity in the binding does not mean
open-ended. `t.__update()` with no arguments throws a
wrong-number-of-parameters error; `t.__update(a, b, c)` copies `a`, then `b`,
then `c`, each overwriting slots the previous ones set.

Despite the name, this is an ordinary method - there is no operator or
assignment syntax that calls it implicitly, script code always writes
`t.__update(other)` out. See [`__merge`](sym:types.Table.__merge) for the
non-mutating version that builds a new table instead of writing into `t`.

## Example

{{example:types.Table.__update}}
