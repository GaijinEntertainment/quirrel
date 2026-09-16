---
params: [other]
see_also: [types.Table.__update, types.Table.clear, types.Table.clone]
---

Replaces the table's entire contents with a copy of `other`'s slots, in
place.

## Parameters

- `other` - table to copy from

## Return value

The table itself, now holding a copy of `other`'s slots.

## Errors

Throws `Cannot modify immutable object` when the table was frozen with
`freeze()`. Throws `parameter 1 of 'replace_with' has an invalid type
'array' ; expected: 'table'` (with the actual type in place of `array`) when
`other` is not a table.

## Notes

Takes exactly one argument, and it must be a `table` - unlike
[`__update`](sym:types.Table.__update), which also accepts a `class` or
`instance`. Existing references to the table being replaced see the new
contents too, since this mutates the same object rather than building a new
one; `other` itself is left untouched.

Every old slot is gone before any slot of `other` is copied in, so a slot that
`other` does not have cannot survive the call.

Replacing a table with itself is a no-op and keeps every slot.

## Example

{{example:types.Table.replace_with}}
