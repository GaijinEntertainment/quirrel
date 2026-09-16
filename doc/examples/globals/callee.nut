let fact = function(n) {
  return n <= 1 ? 1 : n * callee()(n - 1)  // recurse without giving the closure a name
}
println("fact(5) =", fact(5))

// even the top level of a script runs inside a closure, so this never throws here
println("type(callee()) =", type(callee()))
