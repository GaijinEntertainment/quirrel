let types = require("types")
println("types.Float() =", types.Float())        // no args: 0.0, printed with no decimal point
println("types.Float(5) =", types.Float(5))       // integer -> float
println("types.Float(\"3.25\") =", types.Float("3.25"))  // string parse
try { types.Float(null) }
catch (e) println("types.Float(null) throws:", e)
