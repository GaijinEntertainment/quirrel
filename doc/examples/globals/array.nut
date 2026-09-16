let a = array(3)
println("type(a[0]) =", type(a[0]))     // fill value when none is given

let b = array(3, 0)
b[1] = 9
println("b[0] =", b[0])
println("b[1] =", b[1])
println("b[2] =", b[2])

// default_value is stored as is, not cloned per slot
let c = array(2, {n = 0})
c[0].n = 5
println("c[1].n =", c[1].n)

try { array(-1) } catch (e) { println("array(-1) throws:", e) }
