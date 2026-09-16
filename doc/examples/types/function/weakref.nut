local f = function() {}
let w = f.weakref()
println("typeof w, (w.ref() == f) =", typeof w, w.ref() == f)
f = null
println("w.ref() =", w.ref())
