---
see_also: [types.Array.totable, iostream.blob.tostring]
---

Returns the default text representation of this array.

## Return value

A string of the form `(array : 0x...)`; printing an array (with `print` or
`println`) shows this same text.

## Notes

The returned string embeds this array's memory address, so it differs on
every run and must never be printed as is; only its fixed prefix, checked
here, is stable. Every built-in type without its own `_tostring` reports
itself this way; see [iostream.blob.tostring](sym:iostream.blob.tostring)
for the same text on a blob, and for why some types need this method
registered explicitly to reach it.

## Example

{{example:types.Array.tostring}}
