from "async" import Future

// markHandled acknowledges a faulted future without consuming the value (getValue
// still reads it, getState still "faulted"). No "[sqasync] unhandled" line in the
// golden is the assertion that the ack suppressed the report.

// Acknowledge after the fault.
let f = Future()
f.reject("boom")
println($"f: {f.getState()} / {f.getValue()}")
f.markHandled()
println($"f after ack: {f.getState()} / {f.getValue()}")

// Pre-acknowledge before the fault.
let g = Future()
g.markHandled()
g.reject("later")
println($"g: {g.getState()} / {g.getValue()}")

print("script done\n")
