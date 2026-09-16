from "async" import Future

// Acknowledge after the fault: value and state are unchanged.
let f = Future()
f.reject("boom")
f.markHandled()
println("f.getState() / f.getValue() =", $"{f.getState()} / {f.getValue()}")

// Pre-acknowledge before the fault: still no report when it lands.
let g = Future()
g.markHandled()
g.reject("later")
println("g.getState() / g.getValue() =", $"{g.getState()} / {g.getValue()}")
