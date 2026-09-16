---
title: Performance
group: Guides
order: 93
summary: How Quirrel compares with Lua, LuaJIT, Luau, QuickJS and Squirrel 3.
---

As Bob Nystrom put it: most benchmarks are not worth the pixels they are printed
on, but people like them. So here are two sets, both measured on Windows x64 and
both committed with the numbers they produced.

Every row times the benchmarked code alone: not interpreter startup, not
compilation. Each workload repeats the load inside one process and keeps its best
time, because the best time is the one least polluted by whatever else the machine
was doing.

Each cell in Benchmarks below is measured three times over. The harness starts
three processes one after another, all pinned to the same logical CPU, and the
page shows the fastest of the three.

The fastest run is the one to publish. A process can share its core with other
work, or land on an efficiency core, or get an unlucky code alignment. Each of
those makes it slower, and none of them makes it faster. The core alone is worth
tens of percent on this hardware.

The harness also watches how far the three runs are apart. It reports a spread of
more than five percent, as long as the runs also differ by more than five
milliseconds: on a row that takes tens of milliseconds, the clock alone is worth a
few percent. A row it reports is measured again on an idle machine.

LuaJIT is measured with the JIT off. Quirrel, Lua and QuickJS are built for
platforms where generating code at runtime is not allowed - consoles, phones - so
comparing against a JIT would answer a question nobody here can act on.

If you need the fastest interpreter, or an AOT language, the answer is
[Daslang](https://daslang.io/#performance), which has more benchmarks of this kind.
Quirrel, Lua and JS are highly dynamic and much quicker to learn, so those are what
is compared here.

## Benchmarks

Shorter bars are better.

{{benchmarks}}

## VM acceptance

The head-to-head harness used for interpreter work: Quirrel against Lua and Luau
on paired workloads with identical algorithms, general interpreter loads (fib,
binarytrees, life, mandel, strings) and daRg-UI-shaped ones (desc_churn,
probe_storm, nullable_probe, closure_storm, method_calls). Run it before and after
any change to the VM.

Times are milliseconds and the ratio is Quirrel over the other interpreter, so
above 1.0 means Quirrel is slower.

{{vm_benchmarks}}

## What was built, and how

Everything is built for Windows 64-bit with clang-cl where the source allows it.
The Quirrel rows measure the release build of the interpreter from the Dagor
engine tree - the shipped runtime configuration, clang and mimalloc. A build on the
CRT heap, such as a plain cmake `sq.exe`, is up to twice as slow on table-sweep
workloads, which would misrepresent shipped performance rather than measure it.

The third-party interpreters are committed next to their workloads, each a release
console exe that imports KERNEL32 alone. Two of them have to be asked for their
jump-table interpreter loop, because both gate it on `__GNUC__`, which clang-cl
does not define: Lua takes `-DLUA_USE_JUMPTABLE=1`, and in QuickJS the line reading
`#if defined(EMSCRIPTEN) || defined(_MSC_VER)` becomes
`#if defined(EMSCRIPTEN) || (defined(_MSC_VER) && !defined(__clang__))`. Without
that second one the primes workload takes twice as long, which measures the MSVC
command line and not QuickJS.

LuaJIT carries one source change, so that a short repetition can be timed at all:
`os.clock()` reads the performance counter, because the CRT `clock()` ticks at one
millisecond, which is about the whole time a rep takes.

## Sources

Every workload, harness and prebuilt interpreter is in `bench/` next to `doc/` in
the [Dagor engine](https://github.com/GaijinEntertainment/DagorEngine) tree, under
`prog/1stPartyLibs/quirrel/quirrel`, one folder per language: `bench/quirrel`,
`bench/lua`, `bench/luau`, `bench/js`.

- `bench/benchmarks.py` - the cross-language suite above. It writes
  `doc/content/_bench.json`, which this page renders.
- `bench/run_vm_bench.py` - the VM acceptance harness. It writes
  `doc/content/_vm_bench.json`.

Both need the Windows interpreters, so the numbers are refreshed by hand and
committed; the site itself builds anywhere. `bench/README.md` says how to run them
and how each committed binary was built.
