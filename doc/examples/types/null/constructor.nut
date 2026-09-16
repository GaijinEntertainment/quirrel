let types = require("types")
println("types.Null() =", types.Null())
println("types.Null(1, 2, 3) =", types.Null(1, 2, 3)) // extra arguments are accepted and ignored
println("type(types.Null()) =", type(types.Null()))

// unlike clone, "constructor" is not keyword-blocked after a dot
println("null.constructor() =", null.constructor())
