---
see_also: [debug.resurrectunreachable]
---

Runs the garbage collector now, instead of waiting for the VM to run it on its own.

## Return value

The number of collectable objects it found unreachable and freed.

## Notes

Quirrel frees most objects by reference counting as soon as the last reference
goes away; the collector only has work to do for a reference cycle, such as two
tables that each hold a slot pointing at the other. Calling `collectgarbage` does
not create cycles or force one to exist; it only sweeps whatever cycles are
already garbage at the time it runs.

This function, and `resurrectunreachable`, exist only in a build that keeps a
garbage collector. A build compiled with `NO_GARBAGE_COLLECTOR` does not register
either one, so `"collectgarbage" in require("debug")` is false there instead of
the call failing.

## Example

{{example:debug.collectgarbage}}
