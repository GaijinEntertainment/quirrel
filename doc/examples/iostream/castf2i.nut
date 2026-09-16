from "iostream" import castf2i, casti2f

println("castf2i(1.5) =", castf2i(1.5))
println("casti2f(castf2i(1.5)) =", casti2f(castf2i(1.5)))  // round trip returns the original float

// an int argument is converted to a float first, so this is castf2i(1.0),
// not the bit pattern of the integer 1
println("castf2i(1) == castf2i(1.0) =", castf2i(1) == castf2i(1.0))

// a negative float still gives a non-negative int: the sign bit just
// becomes bit 31 of the result
println("castf2i(-1.0) > 0 =", castf2i(-1.0) > 0)
