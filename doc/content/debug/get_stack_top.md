---
see_also: [debug.format_call_stack_string, debug.getstackinfos]
---

Returns the current VM stack top index.

## Return value

How many values are visible on this call's own frame: the implicit `this`,
plus whatever explicit arguments it was called with.

## Notes

This function takes no arguments of its own, so a legal call to it always
returns `1`, the implicit `this`; nothing about how deep the calling chain
goes is visible from inside it. The stack this reports is always relative to
the currently running call, never an absolute count from the bottom of the VM.

## Example

{{example:debug.get_stack_top}}
