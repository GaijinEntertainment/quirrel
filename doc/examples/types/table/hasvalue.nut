let t = {a=1, b=2, c=1}
println("t.hasvalue(2) =", t.hasvalue(2))
println("t.hasvalue(99) =", t.hasvalue(99))

// value equality is raw: two distinct tables with equal contents don't match
let marker = {tag=1}
let u = {slot=marker}
println("u.hasvalue({tag=1}) =", u.hasvalue({tag=1}))   // a different table, not the same one
println("u.hasvalue(marker) =", u.hasvalue(marker))    // the very same table
