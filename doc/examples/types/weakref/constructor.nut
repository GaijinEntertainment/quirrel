let types = require("types")
let t = {a = 1}
let wr = types.WeakRef(t)
println("type(wr) =", type(wr))
println("wr.ref() == t =", wr.ref() == t)

// a non-refcounted value has nothing to weakly reference
println("type(types.WeakRef(5)) =", type(types.WeakRef(5)))
