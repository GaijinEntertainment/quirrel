---
title: The stack
group: Embedding
order: 102
summary: How C and the VM exchange values.
---

C and the VM exchange values through a stack, a design inherited from Lua. To call
a script function you push the function and its arguments and then call; when a
script calls a native, the arguments are on the stack waiting.

The stack exists because the two sides disagree about lifetime. A value the VM
knows about must stay reachable while the collector can run, and a `SQObject` in a C
local is not reachable. Putting it on the stack makes it so.

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

`sq_gettop` returns 4 here: it is both the top index and the number of values.

Prefer negative indexes for something you just pushed and positive ones for a
parameter you were given. Mixing the two in one expression is where off-by-one
errors come from, because a push moves every negative index and no positive one.

## Moving values

`sq_push` copies a value already on the stack to the top. `sq_pop` drops a given
number, `sq_remove` takes one out of the middle, and `sq_settop` forces the size,
padding with nulls if it grows.

The full list is on [Stack operations](page:capi/stack).

## Values in and out

`sq_pushstring`, `sq_pushinteger`, `sq_pushfloat`, `sq_pushbool`,
`sq_pushuserpointer` and `sq_pushnull` put a C value on the stack.

Coming back the other way, `sq_getstring`, `sq_getinteger`, `sq_getfloat`,
`sq_getbool`, `sq_getuserpointer` and `sq_getuserdata` each return `SQRESULT`,
because the value at that index may not be of the type asked for. Quirrel does not
coerce here: a `sq_getstring` on a slot holding an integer fails rather than
formatting it.

`sq_gettype` reports what is actually there, as one of `OT_NULL`, `OT_INTEGER`,
`OT_FLOAT`, `OT_STRING`, `OT_TABLE`, `OT_ARRAY`, `OT_USERDATA`, `OT_CLOSURE`,
`OT_NATIVECLOSURE`, `OT_GENERATOR`, `OT_USERPOINTER`, `OT_BOOL`, `OT_INSTANCE`,
`OT_CLASS` or `OT_WEAKREF`.

A `const char *` obtained from `sq_getstring` belongs to the VM. It is valid only
while the string stays reachable, so a pointer kept past the pop is a dangling one.
Copy it, or hold a reference as [Holding references from C](page:embedding/objects)
describes.
