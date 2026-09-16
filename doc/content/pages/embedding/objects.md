---
title: Holding references from C
group: Embedding
order: 110
summary: Keeping a script value alive after it leaves the stack.
---

A value on the stack lives only as long as it stays there. To keep a script
value across frames - a callback the host will invoke later, a configuration
table read once at startup - take a handle and add a reference to it.

```cpp
HSQOBJECT obj;

sq_resetobject(&obj);       // a handle must be initialized before use
sq_getstackobj(v, -2, &obj);
sq_addref(v, &obj);

// ... the value now survives whatever happens to the stack ...

sq_pushobject(v, obj);      // put it back when it is needed
sq_release(v, &obj);        // and drop the reference when it is not
```

Three common mistakes:

- **`sq_resetobject` is not optional.** An uninitialized `HSQOBJECT` has a
  garbage type tag, and `sq_addref` on it corrupts a refcount that belongs to
  another object. The damage shows up far from the cause.
- **`sq_getstackobj` alone keeps nothing alive.** It copies a handle. Without
  the `sq_addref` the value can be collected while the handle still looks valid.
- **Every `sq_addref` needs its `sq_release`.** A missed release leaks. In a VM
  with the collector compiled out it leaks permanently.

`sq_addref` and `sq_release` are inline wrappers. They check whether the type is
reference counted before they call `sq_addref_refcounted` or
`sq_release_refcounted`. Call those directly only when the type is already known
to be collectable.

## Working through a handle

Once a value is held as a handle, it can be used without the stack. The
`sq_obj_*` family reads and writes through the handle directly:

```cpp
sq_obj_get(v, &table, &key, &out, /*raw*/ false);
```

This saves a push and a pop per access. It matters in a host that reads the same
few script values every frame. These entry points are an addition over Squirrel,
whose API was stack-only.

`sq_objtostring`, `sq_objtointeger`, `sq_objtofloat` and `sq_obj_is_true`
convert a handle without a VM, for code that has a value but no stack at hand.

The full list is on [Raw object handling](page:capi/raw).
