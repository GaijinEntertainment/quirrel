---
title: Tables, arrays and userdata
group: Embedding
order: 108
summary: Making and reading tables, arrays and userdata from C.
---

## Tables

`sq_newtable` pushes a new table. `sq_newslot` creates a slot in the table at an
index, and takes the key and value from the stack. It is the C form of `<-`.
`sq_set` and `sq_get` write and read an existing slot. Both follow the
language's rules: a `_get` [metamethod](page:language/metamethods) runs, and a
frozen table refuses the write.

`sq_rawset` and `sq_rawget` skip the metamethods. A frozen table still refuses
the raw write. Use them when the table is data
whose keys came from outside, for the same reason script code uses `.$`: a
metamethod on data you did not write should not decide what your read means.

`sq_setdelegate` and `sq_getdelegate` attach a delegate to a table. This is
host-only. The script API has no equivalent, so a table with metamethods can
only come from C.

## Arrays

`sq_newarray` pushes an array, and fills it with nulls if given a size.
`sq_arrayappend` and `sq_arraypop` move the value between the stack and the
array. `sq_arrayresize`, `sq_arrayinsert`, `sq_arrayremove` and
`sq_arrayreverse` do what their names say.

`sq_getsize` gives the element count of an array, a table or a string.

## Iterating

`sq_next` is the primitive under `foreach`. Push a null as the starting
iterator. Each successful call then leaves the key at -2 and the value at -1:

```cpp
sq_pushnull(v);                   // the iterator
while (SQ_SUCCEEDED(sq_next(v, -2))) {
  // -1 is the value, -2 is the key

  sq_pop(v, 2);                   // before the next step
}
sq_pop(v, 1);                     // the iterator
```

The usual bug is a missing `sq_pop(v, 2)` inside the loop. The stack grows by
two per element, and the iterator index stops pointing at the container.

Table iteration order is not stable across runs, by design. The VM seeds it,
so a script that depends on the order fails during testing and not in front
of a user.

## The registry table

The registry is a hidden table shared by a VM and all its friend VMs. Only C can
reach it, through `sq_pushregistrytable`. It gives a native library a place to
keep its own state without a key in the root table, where a script could see or
replace it. The standard library keeps its delegates and configuration there.

## Freezing

`sq_freeze` pushes an immutable reference to a value. `sq_freeze_inplace` makes
the value itself immutable. A host shares configuration it does not want edited
by handing the script a frozen table. This is cheaper than a copy on every
access. See [freeze](sym:freeze) for the script side.
