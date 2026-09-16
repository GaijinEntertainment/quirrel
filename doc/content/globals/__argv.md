---
see_also: [getroottable, system.getenv]
---

The command line the host was started with, as an array of strings.

## Notes

This is the host's raw `argv`, not a cleaned list of script arguments. It starts
with the interpreter path, then the script path, and it also carries the
interpreter's own options, so the same script sees a different array depending on
how it was launched. Take the arguments you want by position from the end, or
match them by name; do not assume `__argv` has a fixed length.

Root table names are not visible to module code on their own, so reach it through
[getroottable](sym:getroottable) rather than by writing `__argv` directly. A bare
`__argv` fails to compile with `Unknown variable [__argv]`.

The host installs it, not the VM. An application that embeds Quirrel and never
calls `sqstd_register_command_line_args` has no `__argv`, so treat a
missing slot as normal.

## Example

{{example:__argv}}
