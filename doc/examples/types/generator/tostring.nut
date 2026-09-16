function geny() { yield 1 }
let g = geny()
let s = g.tostring()
println("s.slice(0, 12) =", s.slice(0, 12)) // "(generator :"
try { g.tostring(1) } catch(e) { println("g.tostring(1) throws:", e) }
