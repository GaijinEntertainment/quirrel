---
params: [callback]
see_also: [types.Table.map, types.Table.findvalue, types.Table.each]
---

Combines every value in the table into one result by calling `callback`
repeatedly.

## Parameters

- `callback` - `callback(accum, value, [key], [table])`, called for every
  slot; its return value becomes the next `accum`

## Return value

The final `accum`. On an empty table with no initial value, returns nothing
(`null`) without calling `callback`. On a table with exactly one slot and no
initial value, returns that slot's value directly, again without calling
`callback`.

## Errors

Whatever `callback` throws propagates out of `reduce`.

## Notes

Takes 1 required argument (`callback`) plus 1 truly optional one: an initial
value for `accum`. The VM-reported signature is `reduce(arg1: function,
...)` - the optional initial value has no placeholder of its own, it
is absorbed into the trailing `...`, so the signature undercounts
the real parameter list rather than mislabeling it (contrast
[`findvalue`](sym:types.Table.findvalue), whose optional parameter at least
gets its own bracketed placeholder). Only the `callback` name comes from this
page's front matter for that reason.

Anything passed after the initial value is silently ignored rather than
rejected - `t.reduce(f, 1, "extra")` does not throw, unlike
[`findvalue`](sym:types.Table.findvalue)'s explicit check for too many
arguments.

Without an initial value, the first slot's value seeds `accum` and
`callback` is not called for it - `callback` only runs for the second slot
onward. With an initial value, `callback` runs once per slot, from the
first.

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more - one more than
[`map`](sym:types.Table.map)'s callback, because `accum` comes first.

## Example

{{example:types.Table.reduce}}
