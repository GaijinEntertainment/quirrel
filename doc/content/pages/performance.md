---
title: Performance
group: Guides
order: 93
summary: Benchmarks against Lua, LuaJIT, Luau, QuickJS and Squirrel 3.
---

This page has two sets of benchmarks. Both are measured on Windows x64, and
both are committed with the numbers they produced.

Every row times the benchmarked code only, without interpreter startup and
without compilation. Each workload repeats the load inside one process and keeps
its best time, because the best time has the least noise from other work on the
machine.

Each cell in Benchmarks below is measured three times. The harness starts three
processes one after another, all pinned to the same logical CPU, and the page
shows the fastest of the three.

The page shows the fastest run because noise only makes a run slower. A process
can share its core with other work, land on an efficiency core, or get an
unlucky code alignment. None of these makes it faster. The core alone changes
the result by tens of percent on this hardware.

The harness also checks how far the three runs are apart. It reports a spread of
more than five percent when the runs also differ by more than five milliseconds,
because on a row that takes tens of milliseconds the clock alone accounts for a
few percent. A reported row is measured again on an idle machine.

LuaJIT is measured with the JIT off. Quirrel, Lua and QuickJS are built for
platforms where generating code at runtime is not allowed, such as consoles and
phones, so a comparison against a JIT would not be useful.

If you need the fastest interpreter, or an AOT language, use
[Daslang](https://daslang.io/#performance), which has more benchmarks of this
kind. Quirrel, Lua and JS are highly dynamic and much quicker to learn, so this
page compares those.

## Benchmarks

Shorter bars are better.

{{benchmarks}}

## VM acceptance

This is the head-to-head harness used for interpreter work. It runs Quirrel
against Lua and Luau on paired workloads with identical algorithms: general
interpreter loads (fib, binarytrees, life, mandel, strings) and daRg-UI-shaped
loads (desc_churn, probe_storm, nullable_probe, closure_storm, method_calls).
Run it before and after any change to the VM.

Times are milliseconds. The ratio is Quirrel over the other interpreter, so a
value above 1.0 means Quirrel is slower.

{{vm_benchmarks}}

## What was built, and how

Everything is built for Windows 64-bit with clang-cl where the source allows it.
The Quirrel rows measure the release build of the interpreter from the Dagor
engine tree, which is the shipped runtime configuration: clang and mimalloc. A
build on the CRT heap, such as a plain cmake `sq.exe`, is up to twice as slow on
table-sweep workloads, so it would not measure shipped performance.

The third-party interpreters are committed next to their workloads. Each is a
release console exe that imports KERNEL32 only. Two of them need a change to get
their jump-table interpreter loop, because both gate it on `__GNUC__`, which
clang-cl does not define. Lua takes `-DLUA_USE_JUMPTABLE=1`. In QuickJS the line
`#if defined(EMSCRIPTEN) || defined(_MSC_VER)` becomes
`#if defined(EMSCRIPTEN) || (defined(_MSC_VER) && !defined(__clang__))`. Without
the QuickJS change the primes workload takes twice as long, which measures the
MSVC command line and not QuickJS.

LuaJIT has one source change, so that a short repetition can be timed:
`os.clock()` reads the performance counter, because the CRT `clock()` ticks once
per millisecond, which is about the time one rep takes.

## Sources

Every workload, harness and prebuilt interpreter is in `bench/` next to `doc/`
in the [Dagor engine](https://github.com/GaijinEntertainment/DagorEngine) tree,
under `prog/1stPartyLibs/quirrel/quirrel`, one folder per language:
`bench/quirrel`, `bench/lua`, `bench/luau`, `bench/js`.

- `bench/benchmarks.py` - the cross-language suite above. It writes
  `doc/content/_bench.json`, which this page renders.
- `bench/run_vm_bench.py` - the VM acceptance harness. It writes
  `doc/content/_vm_bench.json`.

Both need the Windows interpreters, so the numbers are refreshed by hand and
committed. The site itself builds anywhere. `bench/README.md` says how to run
them and how each committed binary was built.
