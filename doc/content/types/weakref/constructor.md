---
params: [obj]
see_also: [types.WeakRef.ref, types.WeakRef.weakref, types.Integer.weakref, types.Table.weakref]
---

Returns a weak reference to `obj`. This is what runs when `types.WeakRef`
itself is called, `types.WeakRef(obj)` - the same conversion
`obj.weakref()` performs as a method.

## Parameters

- `obj` - the value to make a weak reference to

## Return value

A `weakref` pointing at `obj`, when `obj` is a reference-counted type
(`table`, `array`, `string`, `class`, `instance`, `function`, `generator`,
`thread`, `weakref` or `userdata`). For anything else - `integer`, `float`,
`bool`, `null` - there is nothing to weakly reference, so `obj` itself comes
back unchanged; see [`types.Integer.weakref`](sym:types.Integer.weakref) for
why.

## Notes

Takes exactly one argument: `types.WeakRef()` and `types.WeakRef(x, y)` both
throw a wrong-number-of-parameters error. The VM cannot show the real name
`obj` here, since this binding carries a type mask instead of a declaration
string and dumps the placeholder `arg1` instead; that placeholder is what
this page's `params:` front matter replaces. The receiver shown,
`(table|userdata|instance|class|null)`, is the same generic fallback
explained on [`types.Integer.weakref`](sym:types.Integer.weakref) - the
binding has no receiver check, only the type check on `obj`.

## Example

{{example:types.WeakRef.constructor}}
