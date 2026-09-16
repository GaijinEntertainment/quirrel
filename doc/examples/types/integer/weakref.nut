// Integers are not reference counted, so weakref() has nothing to point at:
// sq_weakref() just hands the value back unchanged.
let wr = (5).weakref()
println("wr =", wr)
println("type(wr) =", type(wr)) // still "integer", never "weakref"
