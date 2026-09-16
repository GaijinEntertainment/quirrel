---
see_also: [types.Thread.getstatus, types.Table.tostring]
---

Returns a string that names the type and identity of the thread.

## Return value

A `string` shaped like `(thread : 0x...)`, whatever state the thread is in.
The address is the thread's own identity: two threads created from the same
function print two different addresses.

## Notes

Takes no arguments; `t.tostring(1)` throws `wrong number of parameters
passed to native closure 'tostring' (2 passed, 1 required)`.

Because the address changes from run to run, code must never compare or
print `tostring()` of a thread for anything other than a human to eyeball
while debugging.

## Example

{{example:types.Thread.tostring}}
