let t = {a=1, b=2, c=3, d=4}
let evens = t.filter(function(v) { return v % 2 == 0 })
println("evens.len() =", evens.len())      // only "b" and "d" passed the test
println("evens.b =", evens.b)          // the original value is kept, not the callback's

println("t.len() =", t.len())          // the original table is unchanged

// unlike map, filter has no special case for throw null - it aborts
try {
  t.filter(function(v) { if (v == 2) throw null; return true })
} catch (e) {
  println("caught e == null:", e == null)
}
