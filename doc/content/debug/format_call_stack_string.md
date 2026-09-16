---
see_also: [debug.getstackinfos, debug.get_stack_top]
---

Returns a formatted string describing the current call stack.

## Return value

A multi-line string: one `*FUNCTION [name()] source:line` line per frame above
this call, followed by a `LOCALS` section listing the local variables visible
at every one of those frames.

## Example

{{example:debug.format_call_stack_string}}
