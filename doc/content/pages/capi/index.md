---
title: C API reference
group: C API
order: 120
group_index: true
---

Every function the public headers export, grouped as the headers group them.

Signatures are read out of `include/*.h` at build time rather than typed here, so
this list cannot fall behind a header: a function added to `squirrel.h` appears on
the page for its section without anyone editing that page. A description is
authored, and a row that says "not described yet" is a function nobody has written
one for. That is deliberate, for the same reason the script pages mark their gaps:
a missing description you can see beats one you cannot.

Some rows carry a description lifted from the comment above the declaration itself.
Those are the functions whose headers document them better than any second copy
would.

## How to read a signature

`HSQUIRRELVM` is a VM handle, `HSQOBJECT` a handle to a value that can outlive the
stack, and `SQUserPointer` an opaque `void *`. `SQInteger` and `SQFloat` are the
integer and float the VM was built with, which is why `_SQ64` has to be defined the
same way in your project as in the library.

Anything returning `SQRESULT` reports failure the same way, and the two macros are
the only correct test:

```cpp
if (SQ_FAILED(sq_getstring(v, -1, &s)))
  handle_it();
```

An index parameter is a stack index unless the name says otherwise: 1 is the base,
-1 is the top, and 0 is never valid. See [The stack](page:embedding/stack).

## Pages

{{subtopics}}

## See also

- [Embedding Quirrel](page:embedding/index) - how these functions fit together in a host
- [The language](page:language/index) - the semantics the API has to respect
