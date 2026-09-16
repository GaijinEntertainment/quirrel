class Foo { x = 1 }
let inst = Foo()
let wr = inst.weakref()
println("wr.ref() == inst =", wr.ref() == inst)
