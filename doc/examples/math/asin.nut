from "math" import asin

println("asin(0) =", asin(0))
println("asin(0.5) =", asin(0.5))
println("asin(1) =", asin(1))

// out of [-1, 1], the argument is clamped instead of returning nan
println("asin(2) == asin(1) =", asin(2) == asin(1))
println("asin(-2) == asin(-1) =", asin(-2) == asin(-1))
