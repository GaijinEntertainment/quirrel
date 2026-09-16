---
params: [callback, default]
see_also: [types.Table.findindex, types.Table.hasvalue, types.Table.reduce]
---

Returns the first value for which `callback` returns a truthy result.

## Parameters

- `callback` - `callback(value, [key], [table])`, tested against every value
- `default` - value to return when nothing matches

## Return value

The first matching value, or `default` if given and nothing matches,
otherwise `null`.

## Errors

Throws `Too many arguments for findvalue()` when a third argument follows
`callback` and `default` - `findvalue` accepts at most those two. Whatever
`callback` throws also propagates out.

## Notes

Takes 1 required argument (`callback`) plus 1 truly optional one (`default`)
- despite the VM-reported signature ending in `...`, a third argument is a
hard error, not a variadic tail; the check happens before `callback` even
runs. This is the opposite trap from [`reduce`](sym:types.Table.reduce):
there the VM's `...` hides a real optional parameter that never appears in
the signature; here it shows a `...` that is not open-ended.

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more - the same rule as
[`findindex`](sym:types.Table.findindex).

## Example

{{example:types.Table.findvalue}}
