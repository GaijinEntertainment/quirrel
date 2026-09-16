---
see_also: [iostream.blob.writeblob, iostream.stream.readblob, iostream.stream.writestring]
---

Writes all of `blob` at the cursor and advances the cursor by its length.

## Parameters

- `blob` - the source blob; all of it is written

## Return value

The number of bytes written, which is always `blob.len()`.

## Errors

Throws `invalid parameter` when `blob` is an instance but not a blob. Passing
something that is not an instance, such as a number or a string, is
rejected earlier by the declared parameter type and never reaches this
message.

`writeblob` also throws `io error` if the underlying stream cannot store the
full amount, for example a `file` opened read-only; a blob's own storage
never refuses a write, so this case cannot happen on a blob.

## Notes

The same method works on a `file`. Writing past the current `len` grows a
blob to fit, the same as `writen`; writing past the end of a writable file
extends it the same way.

## Example

{{example:iostream.stream.writeblob}}
