---
params: [predicate, default_value]
see_also: [types.Array.findindex, types.Array.filter]
---

Finds the first element for which `predicate` returns a true value.

## Parameters

- `predicate` - `predicate(value, [index], [array])`, tested against each
  element, in order
- `default_value` - returned when nothing matches, instead of `null`

## Return value

The first matching element; `default_value` if nothing matched and it was
given; `null` if nothing matched and it was not.

## Errors

Whatever `predicate` throws, on the first element where it throws.

Throws `Too many arguments for findvalue()` when called with a third real
argument. Unlike [slice](sym:types.Array.slice) or
[sort](sym:types.Array.sort), which silently ignore arguments past what they
use, `findvalue` treats one as a mistake, since a value beyond
`default_value` cannot mean anything.

## Notes

`predicate` gets exactly as many of the arguments listed above as it declares
parameters for, and never more.

## Example

{{example:types.Array.findvalue}}
