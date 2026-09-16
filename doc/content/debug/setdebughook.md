---
see_also: [debug.seterrorhandler]
---

Installs the given function as the VM debug hook; null clears it.

## Parameters

- `hook` - called as `hook(kind, src, line, funcname)` on call, line, and
  return events; `null` clears the hook

## Notes

`kind` is the event's character code, not a one-character string: `'c'` for a
call, `'l'` for a line, `'r'` for a return.

The check for whether to call the hook is compiled into a call's own frame when
that call begins, so installing the hook takes effect only for calls that start
afterward. A call already running when `setdebughook` runs, including the
script that just called `setdebughook`, keeps executing without it.

While the hook itself runs, the VM does not call it again for the hook's own
call and line events, so a hook that runs script code cannot recurse into
itself.

## Example

{{example:debug.setdebughook}}
