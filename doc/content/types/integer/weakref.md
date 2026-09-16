---
see_also: [types.Float.weakref, types.Bool.weakref, types.WeakRef.weakref, types.WeakRef.ref, types.Integer.clone]
---

Returns the integer itself.

## Return value

`this`, unchanged - not a `weakref` value.

## Notes

The signature above, `(table|userdata|instance|class|null).weakref()`, is not
the real receiver: this binding carries no type check (it is
registered identically on every built-in type), so the introspection tool
falls back to a fixed placeholder list of five container-like types for
display. `integer` is not even one of them, which shows that the
list is a fallback and not a restriction; see
[`types.String.constructor`](sym:types.String.constructor) for the same
placeholder on a different binding.

`weakref()` on an integer returns the integer because
`weakref()` only does something for a reference-counted value: it calls
`sq_weakref`, which checks whether the object is reference counted before
creating an `SQWeakRef`, and an integer never is. `float` and `bool` behave
the same way; see [`types.Float.weakref`](sym:types.Float.weakref) and
[`types.Bool.weakref`](sym:types.Bool.weakref). A `weakref` of a `weakref`
is different again, because a `weakref` object itself *is* reference
counted - see [`types.WeakRef.weakref`](sym:types.WeakRef.weakref).

## Example

{{example:types.Integer.weakref}}
