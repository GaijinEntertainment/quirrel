from "math" import atan2, PI

// y comes first, x second - the reverse of a plain division
println("atan2(1, 1) =", atan2(1, 1))
println("atan2(1, 0) =", atan2(1, 0))
println("atan2(0, -1) =", atan2(0, -1))

// atan2(0, 0) is undefined in plain math, but Quirrel defines it as 0
println("atan2(0, 0) =", atan2(0, 0))
