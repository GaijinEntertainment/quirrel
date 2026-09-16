function body() { suspend() }
let t = newthread(body)
println("clone t == t =", clone t == t)
println("t[\"clone\"]() == t =", t["clone"]() == t)
