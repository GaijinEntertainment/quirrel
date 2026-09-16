---
title: C API reference
group: C API
order: 120
group_index: true
---

Every function the public headers export, grouped as the headers group them.

Signatures are read from `include/*.h` at build time, so this list cannot fall
behind a header. A function added to `squirrel.h` appears on the page for its
section without an edit to that page. Descriptions are written by hand. A row
that says "not described yet" has no description yet.

Some rows take their description from the comment above the declaration in the
header, when that comment is the better source.

## How to read a signature

`HSQUIRRELVM` is a VM handle. `HSQOBJECT` is a handle to a value that can outlive
the stack. `SQUserPointer` is an opaque `void *`. `SQInteger` and `SQFloat` are
the integer and float types the VM was built with, so `_SQ64` has to be defined
the same way in your project as in the library.

Every function that returns `SQRESULT` reports failure the same way. Test it with
the two macros only:

```cpp
if (SQ_FAILED(sq_getstring(v, -1, &s)))
  handle_it();
```

An index parameter is a stack index unless the name says otherwise. 1 is the
base, -1 is the top, and 0 is never valid. See [The stack](page:embedding/stack).

## Pages

{{subtopics}}

## See also

- [Embedding Quirrel](page:embedding/index) - how to use these functions in a host
- [The language](page:language/index) - the semantics the API implements
