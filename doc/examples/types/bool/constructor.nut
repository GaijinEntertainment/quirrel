let types = require("types")
println("types.Bool() =", types.Bool())      // no args: false
println("types.Bool(0) =", types.Bool(0))
println("types.Bool(0.0) =", types.Bool(0.0))
println("types.Bool(\"\") =", types.Bool(""))    // an empty string is still truthy
println("types.Bool([]) =", types.Bool([]))    // so is an empty array
println("types.Bool(5) =", types.Bool(5))
