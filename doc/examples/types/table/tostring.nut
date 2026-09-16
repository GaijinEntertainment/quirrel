let t = {a=1}
let s = t.tostring()
// the address varies run to run, so only check the fixed prefix
println("s.slice(0, 8) =", s.slice(0, 8))
println("typeof s =", typeof s)

// two different tables never print the same tostring()
let u = {a=1}
println("t.tostring() == u.tostring():", t.tostring() == u.tostring())
