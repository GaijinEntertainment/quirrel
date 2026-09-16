---
title: Manipulating objects
group: C API
order: 130
summary: Slots, array growth, iteration and freezing.
---

Reading and writing slots, growing arrays, iterating containers, and freezing
them.

Everything here follows the language's own rules: `sq_get` consults a `_get`
[metamethod](page:language/metamethods), `sq_set` consults `_set`, and a frozen
container refuses a write. The `sq_raw*` family on
[Raw object handling](page:capi/raw) skips the metamethods. A frozen container
still refuses a raw write.

`sq_next` is the iteration primitive. Push a null to start. Each step leaves the
key and the value on the stack.

{{capi:object manipulation}}
