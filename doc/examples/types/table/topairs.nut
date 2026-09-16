let t = {a=1, b=2, c=3}
let pairs = t.topairs()
pairs.sort(function(x, y) { return x[0] <=> y[0] })  // sort by key
foreach (p in pairs)
  println($"{p[0]}={p[1]}")

println("pairs.len() == t.len():", pairs.len() == t.len())
