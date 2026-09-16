---
see_also: [types.Table.hasindex, types.Table.tostring]
---

If the table has a delegate providing a `_call` metamethod, returns
introspection info about that metamethod's closure; otherwise returns `null`.

## Return value

`null`, or a table with fields such as `name`, `parameters`, `required_params`
and `varargs` describing the `_call` closure - the same shape a closure's own
`getfuncinfos()` returns.

## Notes

Takes no arguments. This almost always returns `null` for a table
built purely in script: a table's delegate can only be attached with
`sq_setdelegate`, a C API call. Quirrel removed the script-level
`setdelegate()`/`getdelegate()` that Squirrel had, so script code has no way
to give a table a `_call` metamethod, or any other metamethod, of its own.

A slot literally named `_call` on the table itself does nothing here: this
method only looks at the delegate, never at the table's own slots.

## Example

{{example:types.Table.getfuncinfos}}
