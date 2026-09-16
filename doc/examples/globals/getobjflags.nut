let t = {}
println("getobjflags(t) =", getobjflags(t))

let f = freeze(t)
println("getobjflags(f) =", getobjflags(f))
println("getobjflags(t) =", getobjflags(t))   // t's own reference is unaffected by freezing f

println("getobjflags(5) =", getobjflags(5))   // any type is accepted; a scalar simply reports 0
