class Foo { x = 1 }
let wr = Foo.weakref()
println("wr.ref() == Foo =", wr.ref() == Foo)
