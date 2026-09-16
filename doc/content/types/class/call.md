---
see_also: [types.Class.acall, types.Class.pcall, types.Class.instance, types.Function.call]
---

Creates a new instance of the class and runs its constructor, the same as
writing `ClassName(...)`. Exists so a class value obtained indirectly (through a
variable, a table slot, or `getclass()`) can still be instantiated through a
uniform `.call(...)` spelling shared with functions.

## Parameters

- `...` - the constructor's arguments, with one extra leading value that is
  required but ignored (see Notes)

## Return value

The new instance.

## Errors

Whatever the constructor throws propagates uncaught. Passing fewer stack slots
than the constructor needs (counting the ignored leading one) throws `wrong
number of parameters passed to native closure 'constructor' (N passed, M
required)`, naming the constructor, not `call`.

## Notes

`function.call(_this, args...)` takes an explicit `this` as its first argument,
because a plain function's `this` is otherwise whatever the caller bound. `call`
is registered identically for classes, so the same leading slot is required
here too - but a class always builds its own instance to serve as `this`, and
silently overwrites whatever was passed in that slot with it. So
`SomeClass.call(a, b)` does not call the constructor with `(a, b)`: it calls the
constructor with `(b)` alone, having thrown `a` away. Pass `null` as the first
argument to make that visible in the calling code, and see `acall` for the same
rule in array form.

## Example

{{example:types.Class.call}}
