---
see_also: [debug.collectgarbage]
---

Finds every collectable object that is currently unreachable and hands them back
instead of freeing them.

## Return value

An `array` of the unreachable objects it found, or `null` when there are none.

## Notes

This runs the same mark pass as `collectgarbage`, but where `collectgarbage`
would free what it finds, this function links the unreachable objects back into
the VM's live chain and returns them in the array. Holding that array keeps them
alive; once nothing still references it, they are unreachable again and a later
`collectgarbage` reclaims them as usual. Use this to inspect what would be
collected, such as a leaked reference cycle, without destroying it first.

## Example

{{example:debug.resurrectunreachable}}
