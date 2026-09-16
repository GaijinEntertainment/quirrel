---
params: [level]
see_also: [types.Thread.getstatus, debug.getstackinfos]
---

Returns call stack information from inside a suspended thread, for the given
stack level of that thread (not of the caller).

## Parameters

- `level` - how many frames up from the thread's current point to look

## Return value

A table with these keys, or `null` when `level` is out of range:

- `func` - the name of the function at that level
- `src` - its source file
- `line` - the line currently executing in it
- `locals` - a table of its local variables, keyed by name

## Notes

Level `0` here is the thread's own innermost active frame - wherever it is
currently suspended - and `1` is whatever called that, and so on. This
differs from `debug.getstackinfos`, whose level `0` is the call to
`debug.getstackinfos` itself: there is no such frame here, because the call
happens from a different thread than the one being inspected.

Calling this on a thread that has never run (`"idle"`), or with a negative
`level`, returns `null` rather than throwing.

## Example

{{example:types.Thread.getstackinfos}}
