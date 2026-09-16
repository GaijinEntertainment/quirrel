---
see_also: [types.Generator.getstatus, types.Table.tostring]
---

Returns a string that names the type and identity of the generator.

## Return value

A `string` shaped like `(generator : 0x...)`, whatever state the generator is
in. The address is the generator's own identity, not its progress: two
generators created from the same function print two different addresses.

## Notes

Takes no arguments; `g.tostring(1)` throws `wrong number of parameters
passed to native closure 'tostring' (2 passed, 1 required)`.

Because the address changes from run to run, code must never compare or
print `tostring()` of a generator for anything other than a human to eyeball
while debugging.

## Example

{{example:types.Generator.tostring}}
