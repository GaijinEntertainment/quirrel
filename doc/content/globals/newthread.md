---
see_also: [suspend]
---

Creates a thread (a coroutine) that runs `func` when resumed.

## Parameters

- `func` - a script closure to run on the new thread

## Return value

The new thread, suspended before its first instruction. Call
`thread.call(...)` on it to start `func`, and `thread.wakeup(...)` to resume it
after a `suspend` inside `func`.

## Errors

Throws `newthread expects a non-native closure` for a native function such as
`print`: only script closures can run on a thread of their own.

## Example

{{example:newthread}}
