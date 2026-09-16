#forbid-clone-operator
class Foo { x = 1 }
try {
  Foo.clone()
} catch (e) {
  println("Foo.clone() throws:", e)
}
