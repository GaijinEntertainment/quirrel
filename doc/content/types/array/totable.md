---
see_also: [types.Array.tostring, types.Table.keys]
---

Builds a table from the array's elements.

## Return value

A new table.

Whether the array holds `[key, value]` pairs or plain values decides how:
if the first element is a 2-element array, every element must be one, and
each becomes a `key, value` slot in the result. Otherwise every element
must be a string, number, bool or `null`, and each becomes both the key and
the value of its own slot, as if building a set.

## Errors

Throws `totable() expected array of pairs [[key, value], ...], size of the
each pair array must be exactly 2 elements` when the first element is a
pair but a later one is an array of some other size.

Throws `totable() expected array of pairs [[key, value], ...] or array of
simple types ["key1", "key2", ...]` when the two forms above are mixed, or
an element is neither.

## Example

{{example:types.Array.totable}}
