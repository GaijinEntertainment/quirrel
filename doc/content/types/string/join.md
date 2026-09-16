---
params: [arr, filter]
see_also: [types.String.concat, types.String.split]
---

Joins the items of `arr` into one string, with `str` between each pair.

## Parameters

- `arr` - array of items to join; each is converted with `tostring()`
- `filter` - selects which items are kept; see Notes
- `...` - not a real parameter; a third explicit argument throws instead (see Errors)

## Return value

The items of `arr`, converted with `tostring()` and joined with `str` between
each pair. An empty `arr` (or one where every item is filtered out) gives
`""`.

## Errors

Throws `Too many arguments` when called with more than 2 explicit arguments -
unlike most methods on this page, `join` enforces its own upper bound instead
of silently ignoring the extra. Throws a type-check error naming `array` if
`arr` is not an array.

## Notes

Takes 1 or 2 arguments: `filter` is optional. If `filter` is a `bool`, `true`
drops every `null` item and every empty-string item from the result; `false`
(or omitting `filter`) keeps everything. If `filter` is a function, it is
called once per item and only items for which it returns a truthy value are
kept, e.g. `function(item) { return item.len() > 1 }`.

## Example

{{example:types.String.join}}
