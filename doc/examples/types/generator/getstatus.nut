function geny() { yield 1; return null }
let g = geny()
println("g.getstatus() =", g.getstatus())   // suspended before the first resume too
println("resume g =", resume g)
println("g.getstatus() =", g.getstatus())
println("resume g =", resume g)
println("g.getstatus() =", g.getstatus())
try { resume g } catch(e) { println("resume g throws:", e) }
