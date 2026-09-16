let t = {a=1, b=2}
let alias = t          // same table, not a copy
let r = t.clear()
println("t.len() =", t.len())
println("r == t:", r == t)          // clear returns the table itself
println("alias.len() =", alias.len())     // the alias sees the same empty table

let frozen = freeze({a=1})
try {
  frozen.clear()
} catch (e) {
  println("frozen.clear() throws:", e)
}
