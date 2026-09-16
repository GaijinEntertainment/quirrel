---
title: Debug interface
group: C API
order: 136
summary: The hooks a debugger, profiler or coverage tool attaches to.
---

The hooks a debugger, profiler or coverage tool attaches to.

`sq_setdebughook` installs a callback the VM calls as execution moves: on
entering and leaving a call, and on each new source line when line hooks are
enabled. This hook is enough to build a stepping debugger. `sq_stackinfos` fills
in one frame of the call stack. A stack trace is assembled from these frames.

Line information costs something to emit, so a release build usually leaves it
off.

{{capi:debug}}
