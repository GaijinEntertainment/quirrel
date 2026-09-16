let t = {a=1, b=2, c=3}
println("t.reduce(add), no initial =", t.reduce(function(acc, v) { return acc + v }))
println("t.reduce(add), initial 100 =", t.reduce(function(acc, v) { return acc + v }, 100))

let one = {a=42}
local calls = 0
println("one.reduce(add) =", one.reduce(function(acc, v) { calls++; return acc + v }))
println("calls =", calls)             // a single slot short-circuits: callback never runs

println("{}.reduce(add) == null:", {}.reduce(function(acc, v) { return acc + v }) == null)

// extra arguments beyond the initial value are accepted and ignored
println("t.reduce(add, 0, \"unused\") =", t.reduce(function(acc, v) { return acc + v }, 0, "unused"))
