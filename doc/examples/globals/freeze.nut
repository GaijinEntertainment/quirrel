let t = {a = 1}
let u = t          // alias made before freeze
let f = freeze(t)  // a new, immutable reference to the same table

try { f.b <- 2 } catch (e) { println("f.b <- 2 throws:", e) }
u.a = 2               // still allowed: u itself was never frozen
println("f.a =", f.a)          // visible here too: u and f share the same table

println("getobjflags(t) =", getobjflags(t))  // t's own reference was never marked
println("getobjflags(f) =", getobjflags(f))
