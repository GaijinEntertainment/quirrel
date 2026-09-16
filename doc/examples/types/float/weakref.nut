// Floats are not reference counted either, so this returns the value itself.
let wr = (3.5).weakref()
println("wr =", wr)
println("type(wr) =", type(wr))
