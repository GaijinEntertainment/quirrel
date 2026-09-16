class Foo { x = 1 }
let f = Foo()
let same = f.rawset("x", 42)
println("same == f =", same == f, "f.x =", f.x)
try {
  f.rawset("newkey", 1)
} catch (e) {
  println("f.rawset(\"newkey\", 1) throws:", e)
}
