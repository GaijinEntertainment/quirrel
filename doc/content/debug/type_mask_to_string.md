---
see_also: [debug.get_function_info_table, debug.get_function_decl_string]
---

Converts a type mask, such as the ones `get_function_info_table` returns, into
the same text a declaration string would show for it.

## Parameters

- `mask` - a bit set of type flags

## Return value

A string such as `"int"`, `"int|string"`, or `"any"` when every bit is set.
Mask `0` (no type allowed) gives the empty string.

## Notes

Some names in the output are aliases that already cover more than one bit, such
as `number` for "int or float"; the mask for `int|float` prints as `number`,
not as `int|float`.

## Example

{{example:debug.type_mask_to_string}}
