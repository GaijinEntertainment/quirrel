from "math" import sin, PI

println("sin(0) =", sin(0))
println("sin(PI/6) =", sin(PI/6))
println("sin(PI/2) =", sin(PI/2))

// the argument is converted to float, even when it is an integer
println("type(sin(0)) =", type(sin(0)))
