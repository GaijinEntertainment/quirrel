---
title: Function attributes
group: Guides
order: 92
summary: `pure`, `fastcall` and `nodiscard`, and what each one allows the compiler to assume.
---

A signature on these pages can carry up to three attributes. They are part of the
declaration, not of the body. They tell the compiler and the VM what a call may
assume.

Script functions write them in brackets. Natives write them in the declaration
string that the binding registers.

```nut
const function [pure] square(x: int): int { return x * x }
let f = @[pure] (x) x * x
```

## pure

The result depends only on the arguments. The compiler treats a direct call to a
pure function as a constant expression, so it may fold the call or keep the
result as a constant initializer.

Mark a function pure only when it reads no mutable state and writes none. The VM
does not check this. A pure function with a side effect may not be called at all.

Most of `math` is pure. `math.sqrt` and `math.clamp` are pure. `math.rand` is
not, because it changes the seed.

## fastcall

A native calling convention. The compiler emits a dedicated call opcode that
skips the frame setup, so the native runs on the caller's frame.

A fastcall native has to be a leaf. It must not call back into the VM, and it
must push a bounded number of values. Only the binding sets this attribute.
Script code cannot add or remove it. It is shown here because a fastcall native
does not appear in a stack trace as a frame of its own.

## nodiscard

Discarding the return value is an error. A dev build raises
`Discarding return value of function 'name' with 'nodiscard' attribute`. A release
build without runtime type checks does not.

Use it when a discarded result means the call was useless, for example on a
function that returns a new value instead of changing its argument.
