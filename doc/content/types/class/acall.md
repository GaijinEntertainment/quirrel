---
params: [args]
see_also: [types.Class.call, types.Class.pacall, types.Class.instance, types.Function.acall]
---

Creates a new instance of the class, passing it the constructor arguments packed
into an array. Equivalent to `call`, only with the arguments collected into one
array instead of listed one by one.

## Parameters

- `args` - array of constructor arguments; `args[0]` must be present but its
  value is ignored

## Return value

The new instance.

## Errors

Throws a parameter type error if `args` is not an array. Takes exactly one
argument beyond the receiver, so calling it with zero or two throws `wrong
number of parameters passed to native closure 'acall' (N passed, 2 required)`.
Whatever the constructor itself throws propagates uncaught, same as calling
`ClassName(...)` directly.

## Notes

`function.acall(array_args)` documents `array_args[0]` as the required `this`
object. `Class.acall` shares that array shape for consistency with
`Function.acall`, but a class ignores whatever sits at `args[0]`: constructing
an instance always supplies the new instance itself as `this`, so `args[0]` is
only a placeholder and `args[1]` is the constructor's first real argument. Use
`null` there for clarity. `call` has the same leading placeholder
argument; see its Notes for the non-array form.

## Example

{{example:types.Class.acall}}
