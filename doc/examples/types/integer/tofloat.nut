println("(42).tofloat() =", (42).tofloat())
println("type((42).tofloat()) =", type((42).tofloat()))

// SQFloat is a 32-bit float in this build, so an integer with more than
// about 7 significant decimal digits cannot round-trip through tofloat().
let big = 9007199254740993 // 2^53 + 1
println("big.tofloat() =", big.tofloat())
println("big.tofloat().tointeger() == big =", big.tofloat().tointeger() == big)
