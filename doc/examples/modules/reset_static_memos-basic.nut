let modules = require("modules")
local calls = 0
function drawId() { calls += 1; return calls }

for (local i = 0; i < 2; i++)
  println("static(drawId()) =", static(drawId()))   // same call site: cached after the first run

modules.reset_static_memos()
println("static(drawId()) =", static(drawId()))      // cache cleared: fresh id
