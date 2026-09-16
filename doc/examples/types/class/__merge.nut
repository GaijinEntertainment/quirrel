class A { a = 1 }
class B { b = 2 }
let merged = A.__merge(B)
println("merged.a =", merged.a, "merged.b =", merged.b)
println("A.rawin(\"b\") =", A.rawin("b"))
