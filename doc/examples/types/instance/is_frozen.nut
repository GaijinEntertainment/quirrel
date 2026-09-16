class Foo { x = 1 }
let inst = Foo()
println("inst.is_frozen() before freeze =", inst.is_frozen())
let frozenInst = freeze(inst)
println("frozenInst.is_frozen() =", frozenInst.is_frozen())
println("inst.is_frozen() after freeze =", inst.is_frozen())
