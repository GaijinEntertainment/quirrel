---
title: VM, memory and watchdog
group: C API
order: 122
summary: `sq_open`, the allocator, and the watchdog.
---

Creating a VM, its allocator, and the watchdog that stops a runaway script.

## Virtual machine

A host may hold any number of VMs. Each one made with `sq_open` must be closed with
`sq_close`. `sq_newthread` makes a friend VM that shares the parent's globals and
registry; see [Embedding Quirrel](page:embedding/index).

{{capi:vm}}

## Memory allocation

The VM routes its allocations through these, so a host with its own heap can supply
one at build time and account for what the scripts use.

{{capi:mem allocation}}

## Watchdog

The watchdog is what makes an untrusted or merely buggy script safe to run: the
interpreter loop asks the hook periodically whether it may continue, and stops the
script when the answer is no. It needs no second thread.

A hook of your own outranks a timeout the script itself can move, which is what the
sandbox on this site relies on.

{{capi:watchdog}}
