from "math" import pow

println("pow(2, 10) =", pow(2, 10))
println("pow(2.0, 0.5) =", pow(2.0, 0.5))

// integer arguments still give a float result
println("type(pow(2, 3)) =", type(pow(2, 3)))
