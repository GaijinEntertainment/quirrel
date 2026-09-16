let t = {fizz=1}
let r = t.__merge({buzz=2})
println("r == t:", r == t)           // a new table, not t itself
println("r.fizz =", r.fizz, "r.buzz =", r.buzz)
println("\"buzz\" in t:", "buzz" in t)      // t itself never gained the new key

// works on a frozen table too: it only reads t, never writes it
let frozen = freeze({a=1})
println("frozen.__merge({b=2}).b =", frozen.__merge({b=2}).b)
