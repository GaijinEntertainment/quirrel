---
see_also: [debug.get_function_info_table, debug.doc]
---

Returns a function declaration string.

## Parameters

- `func` - the function to describe

## Return value

The same kind of string this site prints above every symbol page, such as
`add(a, b: int): any`.

For a script closure the string is always rebuilt from the function's own
signature. For a native closure that was registered with a declaration string,
this returns that exact string; one that was registered with a type mask
instead gets a string rebuilt from the mask, with placeholder names `arg1`,
`arg2` standing in for the real parameter names, which the mask does not carry.

When `SQ_STORE_DOC_OBJECTS` is disabled, native declaration text is not
retained. Native declarations then use the rebuilt form, which cannot keep
declaration-only parameter names, default values, or the declared return type.

## Errors

`func` must already be a closure or a native closure; passing anything else
fails the parameter type check before this function runs.

## Notes

A script closure always gives a string. A native closure gives a string when
its binding registered a declaration string, or when it has a name to rebuild
one from. A native closure with no declaration string and no name, such as one
made with `sq_newclosure` and never named, gives `null`. Check for `null`
before using the result on a native closure.

## Example

{{example:debug.get_function_decl_string}}
