let a = [1, 2, 3]
let same = a.apply(function(v) { return v * v })

println("same == a:", same == a)           // apply returns this array, not a copy
println("a (squared) =", ", ".join(a.map(@(v) v.tostring())))

// a throw aborts the loop, and the elements already written stay written
let b = [1, 2, 3, 4]
try { b.apply(function(v) { if (v == 3) throw "stop"; return v * 10 }) }
catch (e) { println("apply() throws:", e) }
println("b (after throw) =", ", ".join(b.map(@(v) v.tostring())))
