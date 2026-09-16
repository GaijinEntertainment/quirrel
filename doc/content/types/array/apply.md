---
params: [callback]
see_also: [types.Array.map, types.Array.each]
---

Replaces every element with the result of `callback` on it.

## Parameters

- `callback` - `callback(value, [index], [array])`, called for each element,
  in order

## Return value

This array.

## Errors

Whatever `callback` throws, on the first element where it throws.

## Notes

`callback` gets exactly as many of the arguments listed above as it declares
parameters for, and never more; a native callback that does not fix its own
parameter count is called with all three.

`apply` transforms in place: an element is overwritten as soon as `callback`
returns for it. If `callback` throws partway through, the elements already
processed keep their new values and the rest keep their old ones; the array
is left half-transformed, not rolled back.

Unlike [map](sym:types.Array.map), `apply` does not give `throw null` any
special meaning: a thrown `null` aborts the loop like any other
thrown value.

## Example

{{example:types.Array.apply}}
