let t = {a=1, b=2, c=3}
local total = 0
t.each(function(v) { total += v })     // 1-param: value only
println("total =", total)

let seen = {}
t.each(function(v, k) { seen[k] <- v })  // 2-param: value and key
println("seen.len() == t.len():", seen.len() == t.len())

let r = t.each(function(v) {})
println("r == null:", r == null)                     // each returns nothing
