---
see_also: [debug.get_function_info_table, debug.get_function_decl_string]
---

Returns a documentation string for a function, class, or instance.

## Parameters

- `subject` - the function, class, or instance to look up

## Return value

The docstring attached to `subject`, or `null` when it has none.

An instance has no docstring of its own; passing one returns its class's
docstring instead.

## Errors

`subject` must already be a function, instance, or class; passing
anything else fails the parameter type check before this function runs.

## Notes

Script code attaches a docstring with `@@"..."` as the first thing inside a
function or class body. Table docstrings are not supported:

```nut
function greet() {
  @@"Prints a friendly greeting."
  println("hi")
}
```

A native function only has a docstring when its binding registered one; most
methods on the built-in types (`array`, `string`, ...) do not, so `doc` on them
is `null`.

## Example

{{example:debug.doc}}
