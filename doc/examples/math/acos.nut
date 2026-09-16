from "math" import acos

println("acos(1) =", acos(1))
println("acos(0.5) =", acos(0.5))
println("acos(-1) =", acos(-1))

// out of [-1, 1], the argument is clamped instead of returning nan
println("acos(2) == acos(1) =", acos(2) == acos(1))
println("acos(-2) == acos(-1) =", acos(-2) == acos(-1))
