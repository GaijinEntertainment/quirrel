---
params: [other]
see_also: [types.Table.__update, types.Table.replace_with, types.Table.clone]
---

Returns a new table built from this table and every argument, later
arguments overriding earlier ones.

## Parameters

- `other` - a table, class or instance to copy slots from; repeatable, so
  more than one may be given

## Return value

A new table, seeded with this table's own slots first, then each `other` in
order, each overwriting slots the previous ones set. Neither this table nor
any `other` is modified.

## Errors

Throws a type error when an argument is not a table, class or instance, but
the wording depends on which one: a bad first `other` throws `parameter 1 of
'__merge' has an invalid type '...' ; expected: 'table|class|instance'`,
while a bad second or later argument throws `parameter N of '<unknown>' has
an invalid type '...' ; expected: 'table|class|instance'`, with `N` counting
the table itself as parameter 1. Only the first argument goes through the
VM's own automatic type check, which knows the function's name; the rest are
checked by hand inside `__merge`, which does not pass a name along.

## Notes

Takes 1 or more arguments - this method is variadic, unlike
[`reduce`](sym:types.Table.reduce) or [`findvalue`](sym:types.Table.findvalue)
on this page, where a negative arity in the binding does not mean
open-ended. `t.__merge()` with no arguments throws a
wrong-number-of-parameters error.

Unlike [`__update`](sym:types.Table.__update), `__merge` never touches `t`;
it builds and returns a separate table instead. Being read-only on `t`, it
still works on a table frozen with `freeze()` - there is nothing here for
the immutable check to reject.

## Example

{{example:types.Table.__merge}}
