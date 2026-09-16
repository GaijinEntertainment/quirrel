---
params: [key1, key2]
see_also: [types.Instance.rawset, types.Instance.rawget, types.Table.swap]
---

Swaps the values stored at two of this instance's fields.

## Parameters

- `key1` - the first field name
- `key2` - the second field name

## Return value

The instance itself, so calls can be chained.

## Errors

Throws `the index doesn't exist` if either key is not an existing field of the
instance. On a frozen reference, throws `Cannot modify immutable object`.

## Notes

`Instance.swap`, `types.Table.swap`, and `types.Array.swap` are the same
native function; only the receiver's type differs, and for an array the two
arguments are numeric indices rather than key names.

## Example

{{example:types.Instance.swap}}
