from "async" import Future

let p = Future()
try { p.getValue() } catch (e) { println("pending:", e) }

let f = Future(); f.resolve(42)
println("fulfilled:", f.getValue())

let n = Future(); n.resolve(null)
println("settled null:", (n.getState() == "fulfilled" && n.getValue() == null))

let r = Future(); r.reject("boom")
println("faulted:", r.getValue())   // a pure peek: does not acknowledge the fault
r.markHandled()                        // ack, so it is not reported as unhandled
