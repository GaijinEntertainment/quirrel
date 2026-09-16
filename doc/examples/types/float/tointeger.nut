println("(3.9).tointeger() =", (3.9).tointeger())   // truncates toward zero, it does not round
println("(-3.9).tointeger() =", (-3.9).tointeger())
println("type((3.9).tointeger()) =", type((3.9).tointeger()))

// no base argument here either, unlike string's tointeger(base)
try { (3.9).tointeger(10) }
catch (e) println("(3.9).tointeger(10) throws:", e)
