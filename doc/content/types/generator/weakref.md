---
see_also: [types.Generator.getstatus, types.Table.weakref]
---

Returns a weak reference to the generator.

## Return value

A `weakref` value. Calling its `ref()` method gives the generator back for as
long as something else still holds a strong reference to it; once the
generator's refcount drops to zero, `ref()` returns `null` instead.

## Notes

Takes no arguments. This is a weak reference to the generator value itself.
It is unrelated to the reference a suspended generator keeps on its own
`this`. A suspended generator holds every local variable strongly except
`this`, which it holds only weakly, so `this` reads back as `null` on the
next resume if nothing else has kept it alive:

```nut
function body() { yield typeof this; yield typeof this }
let env = {tag = "e"}
let g = body.call(env) // env becomes the generator's 'this'
resume g                // "table" - env is still alive here
env = null               // drop the only other strong reference
resume g                 // "null": this did not survive being suspended
```

A running generator - between the moment it is resumed and its next `yield`
or `return` - holds `this` strongly like any other local.

## Example

{{example:types.Generator.weakref}}
