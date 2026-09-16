from "async" import Future

let p = Future()
p.resolve(1)
p.resolve(2)               // no-op: already settled
println("p.getValue() =", p.getValue())      // 1

let q = Future()
try { q.resolve(q) } catch (e) { println("q.resolve(q) throws:", e) }   // self-resolve: rejected

async function task() { return 42 }
let t = task()
try { t.resolve(0) } catch (e) { println("t.resolve(0) throws:", e) }   // task-futures settle only via return/throw
