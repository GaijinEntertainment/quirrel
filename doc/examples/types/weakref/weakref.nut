let t = {}
let wr1 = t.weakref()
let wr2 = wr1.weakref() // weakref *is* reference counted, so this is a real one
println("type(wr2) =", type(wr2))
println("wr2.ref() == wr1 =", wr2.ref() == wr1)
