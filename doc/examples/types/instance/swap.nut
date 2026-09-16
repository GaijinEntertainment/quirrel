class Pair { a = 1 b = 2 }
let p = Pair()
p.swap("a", "b")
println("p.a =", p.a, "p.b =", p.b)
try {
  p.swap("a", "missing")
} catch (e) {
  println("p.swap(\"a\", \"missing\") throws:", e)
}
