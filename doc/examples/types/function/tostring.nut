function f() {}
let s = f.tostring()
println("s.slice(0, 11) =", s.slice(0, 11)) // "(function :" - the address itself is not stable
println("typeof s =", typeof s)
try { f.tostring(1) } catch(e) { println("f.tostring(1) throws:", e) }
