// tofloat() on a float is the identity conversion: no rounding happens here,
// unlike Integer.tofloat() converting a value from a wider integer range.
println("(3.5).tofloat() =", (3.5).tofloat())
println("(3.5).tofloat() == 3.5 =", (3.5).tofloat() == 3.5)
println("type((3.5).tofloat()) =", type((3.5).tofloat()))
