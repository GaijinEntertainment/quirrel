from "async" import Future

let p = Future()
p.reject("boom")
println("p.getState() =", p.getState())        // faulted
p.reject("ignored")          // no-op: already settled

async function consume(f) {
  try { await f } catch (e) { println("caught:", e) }
}
consume(p)
println("script done")
