---
title: The stack
group: Embedding
order: 102
summary: How C and the VM exchange values through the stack.
---

C and the VM exchange values through a stack. Quirrel inherited this design from
Lua. To call a script function, push the function and its arguments, then call.
When a script calls a native, the arguments are already on the stack.

The stack solves a lifetime problem. A value the VM knows about must stay
reachable while the collector can run. A `SQObject` in a C local is not
reachable. A value on the stack is reachable.

## Indexes

Every API function that names a position follows the same convention:

- **1** is the base of the current frame.
- **-1** is the top, and negative indexes count down from there.
- **0** is never a valid index.

Given a stack holding `"foo"`, `0.5`, `1`, `"test"` from base to top:

| value | positive | negative |
| --- | --- | --- |
| `"test"` | 4 | -1, the top |
| `1` | 3 | -2 |
| `0.5` | 2 | -3 |
| `"foo"` | 1, the base | -4 |

`sq_gettop` returns 4 here. It is both the top index and the number of values.

Use negative indexes for values you pushed yourself and positive indexes for
parameters you received. A push changes every negative index and no positive
one, so mixing the two in one expression causes off-by-one errors.

## Moving values

`sq_push` copies a value already on the stack to the top. `sq_pop` drops a given
number of values. `sq_remove` takes one value out of the middle. `sq_settop`
sets the size, and pads with nulls when the stack grows.

The full list is on [Stack operations](page:capi/stack).

## Values in and out

`sq_pushstring`, `sq_pushinteger`, `sq_pushfloat`, `sq_pushbool`,
`sq_pushuserpointer` and `sq_pushnull` put a C value on the stack.

`sq_getstring`, `sq_getinteger`, `sq_getfloat`, `sq_getbool`,
`sq_getuserpointer` and `sq_getuserdata` read a value back. Each returns
`SQRESULT`, because the value at that index may not have the type asked for.
Quirrel does not coerce here: `sq_getstring` on a slot that holds an integer
fails. It does not format the integer.

`sq_gettype` reports the type of the value, as one of `OT_NULL`, `OT_INTEGER`,
`OT_FLOAT`, `OT_STRING`, `OT_TABLE`, `OT_ARRAY`, `OT_USERDATA`, `OT_CLOSURE`,
`OT_NATIVECLOSURE`, `OT_GENERATOR`, `OT_USERPOINTER`, `OT_BOOL`, `OT_INSTANCE`,
`OT_CLASS` or `OT_WEAKREF`.

A `const char *` from `sq_getstring` belongs to the VM. It is valid only while
the string stays reachable. A pointer kept after the pop is a dangling pointer.
Copy the string, or hold a reference as
[Holding references from C](page:embedding/objects) describes.
