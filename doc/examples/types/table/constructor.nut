let t = {a=1, b=2}
let r = t.constructor()
println("r.len() =", r.len())       // the result is a brand-new, empty table
println("t.len() =", t.len())       // t itself is untouched
println("r == t:", r == t)

// takes no arguments at all
try {
  t.constructor(1)
} catch (e) {
  println("t.constructor(1) throws:", e)
}
