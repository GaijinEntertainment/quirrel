---
see_also: [system.setenv]
---

Returns the value of the environment variable `name`.

## Parameters

- `name` - name of the environment variable to read

## Return value

The variable's value as a string, or `null` if no variable with that name is
set.

## Notes

A missing variable returns `null` rather than crashing. The C `getenv` call
underneath can itself return a null pointer, and the binding pushes that
pointer straight into `sq_pushstring` without an explicit null check, but
`sq_pushstring` treats a null pointer as "push `null`", so the missing case is
safe.

This signature is the PC one. `sqstdsystem.cpp` builds `getenv`, [setenv](sym:system.setenv)
and [system](sym:system.system) only `#if _TARGET_PC`; on every other target (consoles,
mobile) each is replaced by a stub that throws `getenv() not available for this
platform`. Code meant to run on those targets should treat this function as
absent and catch the error, or avoid it.

## Example

{{example:system.getenv}}
