---
params: [value]
see_also: [types.Array.extend, types.Array.insert, array]
---

Adds one or more values to the end of the array.

## Parameters

- `value` - a value to add; give more than one to add several in one call

## Return value

This array.

## Errors

None of its own; `append()` with no value is a wrong-argument-count
error from the VM, not from this function.

## Notes

This binding is registered with a type mask that only covers the first
argument, so the VM's signature shows one parameter, but `append`
accepts any number of them: `a.append(1, 2, 3)` adds all three in one call
and one resize, unlike `array.push` in some other Squirrel-family languages,
which this build does not have.

## Example

{{example:types.Array.append}}
