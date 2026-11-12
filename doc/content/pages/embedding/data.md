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
language's rules: on an instance a `_get` [metamethod](page:language/metamethods)
runs, and a frozen table refuses the write.

`sq_rawset` and `sq_rawget` skip the metamethods of an instance, and `sq_rawget`
also skips the type methods. A frozen table still refuses the raw write. Use
them when the table is data whose keys came from outside: a type method should
not decide what your read means.

## Arrays

`sq_newarray` pushes an array, and fills it with nulls if given a size.
`sq_arrayappend` pops the top of the stack and appends it to the array.
`sq_arraypop` removes the last element and pushes it if `pushval` is true.
`sq_arrayresize`, `sq_arrayinsert`, `sq_arrayremove` and
`sq_arrayreverse` do what their names say.

`sq_getsize` gives the element count of an array, a table or a string.

## Iterating

`sq_next` walks a table or array one element per call. It is the C form of
`foreach`. Push a null as the starting iterator. Each successful call then
leaves the key at -2 and the value at -1:

```cpp
sq_pushnull(v);                   // the iterator
while (SQ_SUCCEEDED(sq_next(v, -2))) {
  // -1 is the value, -2 is the key

  sq_pop(v, 2);                   // before the next step
}
sq_pop(v, 1);                     // the iterator
```

Table iteration order follows the hash layout, not insertion order, and
changes when the table grows. Do not depend on it. To catch code that does,
build with `SQ_RANDOMIZE_FOREACH` set to 1: the order is then different on
every run.

## The registry table

The registry is a hidden table shared by a VM and all its friend VMs. Only C can
reach it, through `sq_pushregistrytable`. It gives a native library a place to
keep its own state without allowing a script to see it.

## Freezing

A host freezes a table or array before it gives it to scripts when scripts must
read the data but not change it. The immutable flag lives on the reference, not
on the object (see [Bindings and constants](page:language/bindings#freeze)), so
freezing a stack slot does not touch the object or other references to it.

`sq_freeze_inplace` sets the flag on the slot at `idx`. `sq_freeze` leaves the
slot at `idx` as it was and pushes a frozen copy of the reference.
Both functions fail for a type other than table, array, class, instance or
userdata.

See [freeze](sym:freeze) for the script side.
