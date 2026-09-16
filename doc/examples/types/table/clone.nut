let t = {a=1, nested={x=1}}
let c = clone t          // the clone operator runs this method
println("c == t:", c == t)            // a different table
println("c.a =", c.a)
c.a = 2
println("t.a =", t.a)               // unaffected: shallow copy at the top level
println("c.nested == t.nested:", c.nested == t.nested) // but nested tables are shared, not copied

let frozen = freeze({a=1})
println("frozen.is_frozen() =", frozen.is_frozen())
println("(clone frozen).is_frozen() =", (clone frozen).is_frozen())  // the clone is always mutable
