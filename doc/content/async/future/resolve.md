---
see_also: [async.Future.reject, async.Future.getValue, async.Future.getState]
---

Settles a pending future as fulfilled with `value`.

## Parameters

- `value` - the value to fulfil with; `null` when omitted

## Errors

Throws `cannot resolve a future with itself` for `p.resolve(p)`: storing a
future as its own value would root it through the runtime's internal value
table, with nothing left to release it. This is the only cycle this method
guards against: an indirect cycle such as `a.resolve(b); b.resolve(a)` is
not caught, and leaks the pair until the VM closes.

Throws `cannot resolve the future returned by an async function` for a
task-future, the future an `async` function call returns. Its only way to
settle is that function's own `return` or `throw`; every other caller,
including the function's own caller, can only read it.

## Notes

Ignored with no error on a future that has already settled, whether
fulfilled or faulted - see [getState](sym:async.Future.getState).

`value` is stored verbatim: resolving with another `Future` does not adopt
or flatten it. The result is a future that holds a future, and a consumer
peels one level per `await`, as described for
[async](sym:async) return values.

## Example

{{example:async.Future.resolve}}
