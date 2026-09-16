let t = {a=1, b=2, c=3}
let doubled = t.map(function(v) { return v * 2 })
println("doubled.a =", doubled.a, "doubled.b =", doubled.b, "doubled.c =", doubled.c)
println("t.a =", t.a)                     // the original table is unchanged

// throw null inside the callback skips that slot instead of aborting
let skipped = t.map(function(v) { if (v == 2) throw null; return v })
println("skipped.len() =", skipped.len())         // "b" is missing, the others made it through
println("skipped.rawin(\"b\") =", skipped.rawin("b"))
