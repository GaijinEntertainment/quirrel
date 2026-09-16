from "iostream" import swap4

println("swap4(0x11223344) =", swap4(0x11223344))
println("swap4(swap4(0x11223344)) =", swap4(swap4(0x11223344)))  // swapping twice restores the value

// only the low 32 bits count; the discarded high bits do not come back
println("swap4(0x100000000 + 0x11223344) == swap4(0x11223344) =", swap4(0x100000000 + 0x11223344) == swap4(0x11223344))
