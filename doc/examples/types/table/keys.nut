let t = {a=1, b=2, c=3}
let k = t.keys()
k.sort()               // iteration order is not guaranteed, so sort first
local s = ""
foreach (key in k) s += key
println("keys sorted =", s)
println("k.len() == t.len():", k.len() == t.len())

println("{}.keys().len() =", {}.keys().len())
