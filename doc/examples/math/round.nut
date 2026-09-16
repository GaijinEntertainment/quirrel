from "math" import round

println("round(2.4) =", round(2.4))
println("round(2.6) =", round(2.6))

// a halfway value rounds away from zero, not to the nearest even integer
println("round(2.5) =", round(2.5))
println("round(-2.5) =", round(-2.5))
println("type(round(2.5)) =", type(round(2.5)))
