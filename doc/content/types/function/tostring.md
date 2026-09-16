---
see_also: [types.Function.getfuncinfos, types.Table.tostring]
---

Returns a string that names the type and identity of the closure.

## Return value

A `string` shaped like `(function : 0x...)`. Both a script closure and a
native closure print the same `function` tag; nothing in the string tells
them apart. The address is the closure's own identity, not its body: cloning
a closure or binding it to a new environment produces a different address.

## Notes

Takes no arguments; `f.tostring(1)` throws `wrong number of parameters
passed to native closure 'tostring' (2 passed, 1 required)`.

Because the address changes from run to run, code must never compare or
print `tostring()` of a closure for anything other than a human to eyeball
while debugging.

## Example

{{example:types.Function.tostring}}
