---
title: Metamethods
group: Language
order: 57
summary: The seventeen reserved names the VM calls for you.
---

A metamethod is an ordinary method with a reserved name. The VM calls it when an
operation on the object has no built-in meaning: reading a slot that is not there,
adding two instances, printing one. There are seventeen of them, and each has an
exact contract about when it runs, what it receives, and what the VM does with the
value it returns.

## Where a metamethod can live

The VM looks for a metamethod in the object's **delegate**. For an instance the
delegate is its class, so writing a method named `_add` in a class body is all it
takes.

A table or a userdata can carry metamethods too, but only the host can arrange it:
the delegate is set through `sq_setdelegate`, and the script API has no equivalent.
A table built in script has no delegate, so none of its metamethods can ever fire,
however the slots are named. Classes and instances are the carriers a script can
build.

This matters most for `_get`: a plain table simply reports a missing key, and a
`_get` slot in it is inert data.

## All metamethods

{{metamethods}}

`==` and `!=` are missing from that table on purpose; see
[Comparison](page:language/metamethods#comparison) below.

## Reading and writing missing slots

`_get` and `_set` run only after the normal lookup has failed. They share one
protocol for reporting "I do not have that key either":

- **`throw null`** is a clean miss. The VM swallows it, keeps falling back, and
  finally raises its own `the index 'x' (type='string') does not exist`.
- **Any other throw** is a real error. It reaches the caller wrapped as
  `Error in '_get' metamethod: <your value>`.
- **Returning `null`** is not a miss. It is the value `null`, successfully found.

So the return value cannot say "absent" and the throw cannot say "present". This is
what lets a proxy distinguish a key that is genuinely missing from a key whose
lookup went wrong.

{{example:language/metamethods-proxy}}

## Iterating

`_nexti` is asked for keys, not for values. The VM calls it with `null` on the
first step and with the previously returned key after that; returning `null` ends
the loop. Each key it returns is then read back through the ordinary read path, so
an object with a `_nexti` almost always needs a `_get` to match. A key that cannot
be read raises `_nexti returned an invalid idx`.

{{example:language/metamethods-nexti}}

## Arithmetic

The metamethod comes from the left operand and runs with it as `this`. There is
no reversed form: if the left operand is an integer and the right is your instance,
the integer decides, and the call fails with
`arith op - between 'integer' and 'instance'`.

The one exception is an integer literal on the left of `+`. The compiler emits it
as `object + literal`, because that addition is commutative for numbers, so `_add`
does run and still sees the object as `this`. Do not rely on it to make a
non-commutative operator work from either side.

An operator with no metamethod throws instead of falling back to a default, so an
instance that defines `_add` but not `_sub` cannot be subtracted.

{{example:language/metamethods-operands}}

## Comparison

`_cmp` returns an integer: negative if `this` sorts before `other`, zero if they
are equal, positive if it sorts after. Returning anything else raises
`_cmp must return an integer`. It drives `<`, `<=`, `>`, `>=` and `<=>`, and it is
what [sort](sym:types.Array.sort) falls back on with no comparator.

Two limits are easy to trip over:

- **`==` and `!=` do not use it.** They compare raw identity, so two instances with
  the same contents are not equal, and no metamethod can change that. Compare a
  field, or write a named method.
- **Both sides must be the same type.** An instance against an integer raises
  `comparison between instance and '0' (type='integer')` without consulting `_cmp`.

The VM also short-circuits when both sides are the very same object, so comparing
something with itself gives zero without a call.

## Calling and cloning

`_call` makes the object callable. Its first parameter is not the first argument:
it is the `this` of the call site, which the VM passes along the way it does for
every call. The arguments follow it.

`_cloned` runs on the object `clone` has already produced, with the original as
its argument. Since `clone` is shallow, this is the hook for giving the copy its own
nested containers.

{{example:language/metamethods-call-cloned}}

## Slot creation and deletion

`_newslot` intercepts `<-`. On an instance it runs, but it cannot complete the job:
instance storage is a fixed-size block sized when the instance is built, so the slot
still cannot appear. Use it to route the value somewhere else, such as a side table.
On a table it runs only if the table has a delegate and the key is new; assigning
over an existing key is a plain write.

`_delslot` is unreachable from ordinary script. The `delete` operator is forbidden
by default, and [rawdelete](sym:types.Table.rawdelete), which the compiler suggests
instead, is raw and skips metamethods by definition. A host that clears that
language feature gets `delete` back, and then a `_delslot` that runs is responsible
for the removal itself: the VM does not remove the slot as well, and the
metamethod's return value becomes the value of the `delete` expression.

## Type name and text

`_typeof` changes `typeof`, not [type](sym:type). `typeof obj` gives whatever string
the metamethod returns while `type(obj)` still gives `"instance"`, because `type`
ignores metamethods on purpose. See
[Values and types](page:language/types) for which of the two to reach for.

`_tostring` is used when the object is printed, concatenated into a string, or
passed to `sq_tostring` from C. It must return a string. Without it, printing an
instance gives an address.

## _lock

`_lock` is the one metamethod that belongs to the class rather than to its
instances. It runs once, when the class locks: at its first instantiation, when
another class inherits it, or on an explicit
[lock()](sym:types.Class.lock), whichever happens first. `this` is the class object,
and the class can still be modified at that moment, so this is the last chance to
add a member with [newmember](sym:types.Class.newmember).

See [Classes and instances](page:language/classes) for what locking prevents
afterwards.
