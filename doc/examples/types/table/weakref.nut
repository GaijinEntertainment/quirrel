local t = {a=1}
let w = t.weakref()
println("typeof w =", typeof w)
println("typeof w.ref() =", typeof w.ref())     // still alive: gives the table back

// storing the weakref in a container slot auto-unwraps it on read
let holder = {}
holder.slot <- w
println("typeof holder.slot =", typeof holder.slot) // not "weakref"

t = null                    // drop the only strong reference
println("w.ref() == null:", w.ref() == null)    // the table is gone now
