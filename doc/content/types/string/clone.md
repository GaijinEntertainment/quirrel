---
see_also: [types.String.weakref, types.String.constructor]
---

Returns `str` itself.

## Return value

`str` unchanged. A Quirrel string is an immutable value, so cloning one has
nothing to copy: the VM's `Clone` falls through to its `default:` case, which
returns the same object instead of allocating a copy (contrast a table or an
array, which do allocate a new one).

## Notes

Takes no arguments.

`clone` is a reserved word (it is also the unary `clone x` operator), and the
parser only accepts an identifier after `.`, so `str.clone()` fails to
compile with `expected 'IDENTIFIER'`. Reach this delegate method through a
computed field access instead: `str["clone"]()`. `types.String.constructor`
is a reserved word too but the parser special-cases it, so that one reads
normally as `str.constructor`.

## Example

{{example:types.String.clone}}
