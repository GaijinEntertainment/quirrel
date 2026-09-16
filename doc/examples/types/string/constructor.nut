let types = require("types")

// types.String(value) is what runs when you call the class directly.
println("types.String(123) =", types.String(123))
println("types.String(true) =", types.String(true))
println("types.String(null) =", types.String(null))

// exactly one argument: none, or more than one, is an error.
try { types.String() } catch (e) println("types.String() throws:", e)
