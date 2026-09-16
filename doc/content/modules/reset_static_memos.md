---
see_also: [modules.on_module_unload, modules.get_native_module_names]
---

Clears every cached result of a `static` expression, everywhere in the
currently loaded modules.

## Return value

`null`.

## Notes

A `static expr` caches the result of `expr` the first time that code
location runs, then reuses the cached value on every later execution without
evaluating `expr` again. `reset_static_memos()` discards those cached values
(recursively, including nested closures) for every loaded module, so the next
execution of each `static` site recomputes `expr` and caches the new result.

Call it after replacing script code that a `static` expression depends on,
such as during a hot reload; it is not meant for a per-frame call, only for
an infrequent one, since it walks every loaded module's closures.

## Example

{{example:modules.reset_static_memos}}
