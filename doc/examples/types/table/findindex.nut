let t = {a=1, b=2, c=3}
// exactly one slot matches, so the result does not depend on visit order
println("t.findindex(v == 2) =", t.findindex(function(v) { return v == 2 }))
println("t.findindex(k == \"a\") =", t.findindex(function(v, k) { return k == "a" }))
println("t.findindex(v > 100) == null:", t.findindex(function(v) { return v > 100 }) == null)

// three parameters: (value, key, table itself)
println("t.findindex(tbl==t && v==3) =", t.findindex(function(v, k, tbl) { return tbl == t && v == 3 }))
