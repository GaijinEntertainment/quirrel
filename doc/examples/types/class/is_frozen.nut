class Foo { x = 1 }
println("Foo.is_frozen() before freeze =", Foo.is_frozen())
let frozenFoo = freeze(Foo)
println("frozenFoo.is_frozen() =", frozenFoo.is_frozen())
println("Foo.is_frozen() after freeze =", Foo.is_frozen())
