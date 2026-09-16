function geny() { yield 1 }
local g = geny()
let w = g.weakref()
println("typeof w, (w.ref() == g) =", typeof w, w.ref() == g)
g = null
println("w.ref() =", w.ref())
