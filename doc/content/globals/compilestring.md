---
see_also: [getroottable]
---

Compiles `source` into a callable closure, without running it.

## Parameters

- `source` - Quirrel source code
- `name` - shown in a stack trace and in diagnostics in place of a file name;
  defaults to `"unnamedbuffer"`
- `bindings` - names visible inside `source`

## Return value

The compiled closure. Call it to run the code.

## Errors

Throws a plain string on a syntax error, such as `end of statement expected
(; or lf)`. Unlike compiling a file, the message carries no line or column,
so a caller that wants a location has to search `source` for it.

## Notes

A name found in `bindings` is baked into the compiled closure as a constant at
compile time, the same way an imported name is: `source` cannot see later
writes to `bindings`, only the value each name held while `source` was
compiled.

Every base library function is added to `bindings` before compiling,
overwriting any entry already using that name, so `source` always has
`print`, `assert` and the rest, however `bindings` was built, and cannot have
them shadowed.

## Example

{{example:compilestring}}
