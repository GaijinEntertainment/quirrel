---
see_also: [iostream.blob.as_string]
---

Exists so `.tostring()` can be called directly on a blob.

## Return value

A string of the form `(instance : 0x...)`, the default text the VM prints
for any instance that has no `_tostring` metamethod.

## Notes

A blob defines `_get`, so without this method a plain `.tostring` lookup
would go through `_get` (which expects a numeric index) instead of reaching
the built-in conversion; this method is registered directly so the call
still works and reaches the same default text that `println` on a blob
would show.

The returned string embeds this blob's memory address, so it differs on
every run and must never be printed as-is; only its fixed prefix, checked
here, is stable. Use `as_string` to get the bytes themselves as text.

## Example

{{example:iostream.blob.tostring}}
