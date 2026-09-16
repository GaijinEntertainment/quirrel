from "iostream" import swap2

println("swap2(0x1234) =", swap2(0x1234))
println("swap2(swap2(0x1234)) =", swap2(swap2(0x1234)))  // swapping twice restores the value

// only the low 16 bits count; the discarded high bits do not come back
println("swap2(0x00011234) == swap2(0x1234) =", swap2(0x00011234) == swap2(0x1234))
