---
title: Manipulating objects
group: C API
order: 130
summary: Slots, array growth, iteration and freezing.
---

Reading and writing slots, growing arrays, iterating containers, and freezing them.

Everything here goes through the language's own rules: `sq_get` consults a
`_get` [metamethod](page:language/metamethods), `sq_set` consults `_set`, and a
frozen container refuses a write. The `sq_raw*` family on
[Raw object handling](page:capi/raw) is the version that does not.

`sq_next` is the iteration primitive: push a null to start, and it leaves the key
and the value on the stack for each step.

{{capi:object manipulation}}
