function geny() { yield 1 }
let g = geny()
println("clone g == g =", clone g == g)
println("g[\"clone\"]() == g =", g["clone"]() == g)
