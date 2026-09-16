from "async" import Future

// reject() with no argument faults with null; getState distinguishes it from a
// pending future (whose getValue throws).

async function consume(fut) { try { let _ = await fut } catch (_) {} }

let r = Future()
r.reject()
println($"faulted state: {r.getState()}")                  // faulted
println($"getValue is null: {r.getValue() == null}") // true
consume(r)   // mark the fault handled

let p = Future()
println($"pending state: {p.getState()}")                  // pending
try { let _ = p.getValue(); print("BUG: pending no throw\n") }
catch (_) { print("pending getValue throws\n") }

print("script done\n")
