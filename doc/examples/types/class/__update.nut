class A { a = 1 }
class B { b = 2 }
let same = A.__update(B)
println("same == A =", same == A)
println("A.a =", A.a, "A.b =", A.b)
