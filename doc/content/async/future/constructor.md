---
see_also: [async.Future.getState, async.Future.resolve, async.Future.reject]
---

Constructs a fresh pending `Future`. There is no JS-style executor callback
and no arguments; settle it later with `resolve` or `reject`.

## Errors

Throws `Future: constructor called more than once` if the constructor runs
twice on the same instance - normally unreachable, since a `new` only ever
calls it once, but reachable from a subclass whose own constructor calls
`base.constructor()` more than once.

## Notes

A subclass of `Future` works as long as its constructor chains to
`base.constructor()`. One that skips it leaves the instance without the
internal state every other method needs; `getState`, `resolve` and the rest
then each throw their own `... invalid 'this'` error instead of working on
a half-built future.

## Example

{{example:async.Future.constructor}}
