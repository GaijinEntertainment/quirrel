let t = {a=1, b=2}
let r = t.swap("a", "b")
println("r == t:", r == t)
println("t.a =", t.a, "t.b =", t.b)

try {
  t.swap("a", "missing")
} catch (e) {
  println("t.swap(\"a\", \"missing\") throws:", e)
}

let frozen = freeze({a=1, b=2})
try {
  frozen.swap("a", "b")
} catch (e) {
  println("frozen.swap(\"a\", \"b\") throws:", e)
}
