from "iostream" import casti2f, castf2i

println("casti2f(castf2i(2.5)) =", casti2f(castf2i(2.5)))  // round trip returns the original float

// only the low 32 bits matter; a wider integer's upper bits are ignored
println("casti2f(0x89) == casti2f(0x100000089) =", casti2f(0x89) == casti2f(0x100000089))
