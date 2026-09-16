let t = {a=1}
let r = t.rawset("b", 2)  // creates a new slot, unlike plain '=' below
println("r == t:", r == t)
println("t.b =", t.b)

try {
  t.c = 3                   // '=' requires the slot to already exist
} catch (e) {
  println("t.c = 3 throws:", e)
}

let frozen = freeze({a=1})
try {
  frozen.rawset("a", 2)
} catch (e) {
  println("frozen.rawset(\"a\", 2) throws:", e)
}
