from "async" import Future

// The one remaining cycle guard: a direct self-settle throws and leaves the
// future pending (storing a future as its own value would root it via the refs
// table and leak). Both the resolve and reject paths are guarded.

let p = Future()
try { p.resolve(p); print("BUG: self-resolve no throw\n") }
catch (e) { println($"self-resolve: {e}") }
println($"p after self-resolve: {p.getState()}")        // pending

let q = Future()
try { q.reject(q); print("BUG: self-reject no throw\n") }
catch (e) { println($"self-reject: {e}") }
println($"q after self-reject: {q.getState()}")          // pending

print("script done\n")
