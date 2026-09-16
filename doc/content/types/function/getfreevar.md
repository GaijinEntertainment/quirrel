---
params: [index]
see_also: [types.Function.getfuncinfos, types.Function.bindenv]
---

Returns the name and current value of one of the closure's free variables
(the outer locals it captured when it was created).

## Parameters

- `index` - which free variable to read, `0` to `freevars - 1`

## Return value

A table with two keys: `name`, the free variable's name as written in the
source, and `value`, its current value.

## Errors

Throws `Invalid free variable index` when `index` is negative or at least as
large as the closure's `freevars` count (see `getfuncinfos`), including on a
closure that captured no free variables.

## Notes

A native closure ordinarily has no free variables to report, so any `index`
throws on one; the rare native closure built with bound outer values (only
possible from the C API) reports each one with the placeholder name
`@NATIVE` instead of a real source name.

Reading a free variable does not affect it: two closures that captured the
same outer local still share it, and a later assignment inside either one is
visible through `getfreevar` on both.

## Example

{{example:types.Function.getfreevar}}
