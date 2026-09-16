#forbid-clone-operator
class Foo {
  x = 1
  // this is the new instance; the one explicit argument is the original
  function _cloned(old) { this.x = old.x * 10 }
}
let a = Foo()
let b = a.clone()
println("a.x =", a.x, "b.x =", b.x, "a != b =", a != b)
