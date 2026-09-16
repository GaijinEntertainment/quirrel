---
params: [value]
see_also: [types.Array.hasvalue, types.Array.indexof]
---

Reports whether any element equals `value`.

## Parameters

- `value` - the value to look for

## Return value

`true` if some element equals `value`, `false` otherwise.

## Notes

Equality is the same raw comparison `==` uses: a number compares by value
against another number, and anything else compares by identity, so two
distinct tables or arrays with the same contents are never equal here. No
`_cmp` or `_eq` metamethod is consulted.

[hasvalue](sym:types.Array.hasvalue) is this same function registered under
a second name.

## Example

{{example:types.Array.contains}}
