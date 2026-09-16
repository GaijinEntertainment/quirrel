let modules = require("modules")
local calls = 0
function computeOnce() { calls++; return calls }

function useMemo() {
  // Same code location every call, so `static` caches after the first run.
  println("static(computeOnce()) sequence:")
  for (local i = 0; i < 3; i++)
    println(static(computeOnce()))
}

useMemo() // prints 1, 1, 1: computeOnce() ran only on the first pass
modules.reset_static_memos()
useMemo() // prints 2, 2, 2: the cache was cleared, so it recomputed once more
