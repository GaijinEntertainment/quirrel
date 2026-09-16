---
see_also: [debug.setdebughook]
---

Installs the given function as the VM error handler; null clears it.

## Parameters

- `handler` - called as `handler(error)`, or `handler(error, trace)` when it
  takes three parameters or a trailing `...`; `null` clears the handler

## Notes

The handler runs when an error unwinds all the way past the last script call on
the stack, uncaught by any `try`/`catch` in between. It is a notification, not a
replacement for `catch`: after the handler returns, the error still propagates
to whatever native code made that call, such as a `thread.call()` from another
part of the script, as if no handler had run.

`trace` is `null` unless the caller that raised the error supplied one.

## Example

{{example:debug.seterrorhandler}}
