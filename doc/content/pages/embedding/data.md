---
title: Tables, arrays and userdata
group: Embedding
order: 108
summary: Making and reading tables, arrays and userdata from C.
---

## Tables

`sq_newtable` pushes a fresh table. `sq_newslot` creates a slot in the table at an
index, taking the key and value from the stack, which is the C form of `<-`.
`sq_set` and `sq_get` write and read an existing slot, and both go through the
language's rules: a `_get` [metamethod](page:language/metamethods) runs, a frozen
table refuses the write.

`sq_rawset` and `sq_rawget` are the versions that skip all of that. Reach for them
when the table is data whose keys came from outside, for the same reason script code
reaches for `.$`: a metamethod on data you did not write should not decide what your
read means.

`sq_setdelegate` and `sq_getdelegate` attach a delegate to a table. This is
host-only: the script API has no equivalent, so a table with metamethods can only
come from C.

## Arrays

`sq_newarray` pushes one, filling with nulls if given a size. `sq_arrayappend` and
`sq_arraypop` take the value from and to the stack; `sq_arrayresize`,
`sq_arrayinsert`, `sq_arrayremove` and `sq_arrayreverse` do what their names say.

`sq_getsize` gives the element count of an array, a table or a string.

## Iterating

`sq_next` is the primitive under `foreach`. Push a null as the starting iterator,
then each successful call leaves the key at -2 and the value at -1:

```cpp
sq_pushnull(v);                   // the iterator
while (SQ_SUCCEEDED(sq_next(v, -2))) {
  // -1 is the value, -2 is the key

  sq_pop(v, 2);                   // before the next step
}
sq_pop(v, 1);                     // the iterator
```

Forgetting the `sq_pop(v, 2)` inside the loop is the usual bug: it grows the stack
by two per element and the iterator index stops pointing at the container.

Table iteration order is deliberately not stable across runs. The VM seeds it, so a
script that depends on the order fails visibly during testing rather than in front
of a user.

## The registry table

The registry is a hidden table shared by a VM and all its friend VMs, reachable only
from C through `sq_pushregistrytable`. It exists so a native library has somewhere
to keep its own state - the standard library keeps its delegates and configuration
there - without putting a key in the root table where a script could see or replace
it.

## Freezing

`sq_freeze` pushes an immutable reference to a value, and `sq_freeze_inplace` makes
the value itself immutable. Handing a script a frozen table is how a host shares
configuration it does not want edited, and it is cheaper than copying on every
access. See [freeze](sym:freeze) for the script side.
