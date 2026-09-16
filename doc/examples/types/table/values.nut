let t = {a=1, b=2, c=3}
let v = t.values()
v.sort()               // iteration order is not guaranteed, so sort first
local s = ""
foreach (val in v) s += val
println("values sorted =", s)
println("v.len() == t.len():", v.len() == t.len())

// keys() and values() correspond when called back to back
let k = t.keys()
let vals = t.values()
local ok = true
for (local i = 0; i < k.len(); i++)
  if (t[k[i]] != vals[i]) ok = false
println("keys/values correspond:", ok)
