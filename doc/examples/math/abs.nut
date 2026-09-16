from "math" import abs

println("abs(-3) =", abs(-3))
println("type(abs(-3)) =", type(abs(-3)))

// the result keeps the argument's type: a float argument gives a float back
println("abs(-3.0) =", abs(-3.0))
println("type(abs(-3.0)) =", type(abs(-3.0)))
