class Foo { x = 1 }
let same = Foo.rawset("x", 99)
println("same == Foo =", same == Foo, "Foo.x =", Foo.x)
Foo.lock()
try {
  Foo.rawset("x", 5)
} catch (e) {
  println("Foo.rawset(\"x\", 5) throws:", e)
}
