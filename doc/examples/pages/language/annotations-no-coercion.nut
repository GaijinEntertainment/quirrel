function identity(x: number): number {
  return x
}
println("type(identity(10)) =", type(identity(10)))     // stays integer: the annotation only widens what is accepted
println("type(identity(10.0)) =", type(identity(10.0)))   // stays float

function damageAt(range: number): float {
  return 100.0 - range          // the subtraction itself promotes the result to float
}
println("type(damageAt(10)) =", type(damageAt(10)))
