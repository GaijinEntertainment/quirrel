from "iostream" import swapfloat

// swapping twice restores the value; that is the only predictable case,
// so the example checks the round trip instead of a single swap
println("swapfloat(swapfloat(1.5)) == 1.5 =", swapfloat(swapfloat(1.5)) == 1.5)

// an int argument is converted to a float first, like castf2i
println("swapfloat(2) == swapfloat(2.0) =", swapfloat(2) == swapfloat(2.0))
