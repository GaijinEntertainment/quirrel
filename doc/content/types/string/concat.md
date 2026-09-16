---
params: [value]
see_also: [types.String.join, types.String.subst]
---

Joins its arguments into one string, with `str` between each pair.

## Parameters

- `value` - the first item to join
- `...` - further items to join

## Return value

`value` and the rest, each converted with `tostring()`, joined with `str`
between each pair. An item can be any type, not only a string.

## Notes

Takes 1 or more arguments: at least one item is required. This is a real,
unbounded vararg - unlike [types.String.join](sym:types.String.join), which
takes its items from an array and caps itself at one extra argument, `concat`
takes the items directly as arguments and accepts any number of them.

## Example

{{example:types.String.concat}}
