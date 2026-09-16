---
params: [other]
see_also: [types.Class.__update, types.Class.newmember, types.Table.__merge]
---

Builds a new class out of the class it is called on plus every argument given,
and returns it. The class it is called on is not changed.

## Parameters

- `other` - a table, class, or instance to merge in
- `...` - more tables, classes, or instances to merge in, in order

## Return value

A new class with no base. It receives, in order, every own member of the
receiver, then of `other`, then of each further argument; a later source
overwrites a same-named member from an earlier one.

## Errors

Throws a parameter type error naming the argument position when a source is not
a table, class, or instance. Takes at least one argument beyond the receiver;
calling it with none throws `wrong number of parameters passed to native closure
'__merge' (1 passed, at least 2 required)`.

## Notes

`Class.__merge` and `table.__merge` are the same native function; only the
receiver's type differs. It reads each source with a plain (non-raw) iteration,
so a source's own `_nexti` or `_get` do not run, but every member lands in the
new class through direct slot creation, not through `_newslot`.

## Example

{{example:types.Class.__merge}}
