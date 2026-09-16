function f() {}
let g = clone f
println("f == g =", f == g)
let h = f["clone"]()
println("f == h =", f == h)
