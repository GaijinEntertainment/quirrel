---
see_also: [getobjflags, deduplicate_object]
---

Returns a reference to `obj` with the immutable flag set.

## Parameters

- `obj` - array, table, instance, class, or userdata

## Return value

A reference to the same object `obj` names, with `getobjflags` reporting the
immutable flag on that reference. `obj` is not changed itself.

## Errors

Throws `Cannot freeze <type>` for any type other than array, table, instance,
class or userdata, for example an `int` or a `function`.

## Notes

The immutable flag lives on the reference, not on the table or array itself:
`freeze` returns a new reference and leaves the passed-in reference
unchanged. Any other existing reference to the same object - a variable it
was copied to earlier, or a value already stored in another table - is
unaffected and can still write through. Assign the result back over `obj`
when every access should go through the frozen reference (`t = freeze(t)`).

Content is unaffected by which reference reads it: a write through a
still-mutable reference is visible through a frozen one too, since both name
the same object. See also [is_frozen](sym:types.Table.is_frozen) and its
array and instance counterparts, which check this same per-reference flag.

## Example

{{example:freeze}}
