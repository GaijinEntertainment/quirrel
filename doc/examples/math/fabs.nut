from "math" import fabs

println("fabs(-3.5) =", fabs(-3.5))
println("fabs(3.5) =", fabs(3.5))

// unlike abs, fabs always returns a float, even for an integer argument
println("fabs(-3) =", fabs(-3))
println("type(fabs(-3)) =", type(fabs(-3)))
