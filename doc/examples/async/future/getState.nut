from "async" import Future

let p = Future()
println("p.getState() =", p.getState())    // pending
p.resolve(1)
println("p.getState() =", p.getState())    // fulfilled

let q = Future()
q.reject("boom")
println("q.getState() =", q.getState())    // faulted
q.markHandled()          // ack so the fault is not reported as unhandled
