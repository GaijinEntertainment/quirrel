---
title: Function attributes
group: Guides
order: 92
summary: `pure`, `fastcall` and `nodiscard`: what each one promises.
---

A signature on these pages can carry up to three attributes. They are part of the
declaration, not of the body: they tell the compiler and the VM what a call is
allowed to assume.

Script functions spell them in brackets, natives spell them in the declaration
string that the binding registers.

```nut
const function [pure] square(x: int): int { return x * x }
let f = @[pure] (x) x * x
```

## pure

The result depends only on the arguments. The compiler treats a direct call to a
pure function as const-scored, so it may fold the call or keep the result as a
constant initializer instead of calling again.

Mark a function pure only when it reads no mutable state and writes none. The VM
does not check the claim, so a pure function with a side effect gives a call that
may silently not happen.

Most of `math` is pure: `math.sqrt` and `math.clamp` are, `math.rand` is not,
because it moves the seed on.

## fastcall

A native calling convention. The compiler emits a dedicated call opcode that skips
the frame setup, so the native runs on the caller's frame.

A fastcall native has to be a leaf: it must not call back into the VM, and it must
push a bounded number of values. Only the binding decides this; nothing in script
can add or remove it. It is visible here because it explains why some natives
cannot appear in a stack trace of their own.

## nodiscard

Throwing the return value away is an error. A dev build raises
`Discarding return value of function 'name' with 'nodiscard' attribute`; a release
build without runtime type checks does not.

Use it where discarding the result means the call was pointless, as with a function
that returns a new value instead of changing its argument.
