function body() { suspend() }
local t = newthread(body)
let w = t.weakref()
println("typeof w, (w.ref() == t) =", typeof w, w.ref() == t)
t = null
println("w.ref() =", w.ref())
