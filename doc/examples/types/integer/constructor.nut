let types = require("types")
println("types.Integer() =", types.Integer())          // no args: 0
println("types.Integer(3.9) =", types.Integer(3.9))       // truncates toward zero, like tointeger()
println("types.Integer(\"2A\", 16) =", types.Integer("2A", 16))  // string parse with an explicit base
println("types.Integer(true) =", types.Integer(true))      // bool -> 1
try { types.Integer({}) }
catch (e) println("types.Integer({}) throws:", e)
