let t = {a=1, b=2, c=3}
println("t.len() =", t.len())

t.rawdelete("a")
println("t.len() after rawdelete(\"a\") =", t.len())

let empty = {}
println("empty.len() =", empty.len())

try {
  t.len(1)     // takes no arguments
} catch (e) {
  println("t.len(1) throws:", e)
}
