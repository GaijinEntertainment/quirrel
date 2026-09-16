let t = {a=1, b=2, c=3}
println("t.findvalue(v == 2) =", t.findvalue(function(v) { return v == 2 }))
println("t.findvalue(v > 100) == null:", t.findvalue(function(v) { return v > 100 }) == null)
println("t.findvalue(v > 100, \"none\") =", t.findvalue(function(v) { return v > 100 }, "none"))  // default value

try {
  t.findvalue(function(v) { return false }, "none", "extra")
} catch (e) {
  println("findvalue with extra arg throws:", e)   // a real error, not a variadic tail
}
