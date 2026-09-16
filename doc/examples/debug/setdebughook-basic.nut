from "debug" import setdebughook

local calls = 0
setdebughook(function(_kind, _src, _line, _name) { calls += 1 })

// a fresh thread call gives the hook a call boundary; this top-level script,
// already running, would not be seen
let worker = newthread(function() { return 1 })
worker.call()
setdebughook(null)

println("calls > 0 =", calls > 0)
