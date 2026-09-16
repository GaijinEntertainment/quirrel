from "async" import Future

// Script-side reject faults the future with the given value; getValue reads it
// back; awaiting throws it. reject after settle is a no-op, and a task-future
// cannot be rejected externally.

let p = Future()
p.reject("nope")
println($"state: {p.getState()}")             // faulted
println($"getValue: {p.getValue()}")           // nope

async function awaitIt() {
    try { let _ = await p; print("BUG: no throw\n") }
    catch (e) { println($"caught: {e}") } // nope
}
awaitIt()

let q = Future()
q.resolve(1)
q.reject("late")                               // no-op, already settled
println($"q state: {q.getState()} value: {q.getValue()}")             // fulfilled 1

async function task() { return 5 }
let t = task()
try { t.reject("x"); print("BUG: no throw\n") }
catch (e) { println($"task reject threw: {e}") }

print("script done\n")
