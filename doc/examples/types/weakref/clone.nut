let wr = {}.weakref()
let wr2 = wr["clone"]()
println("wr2 == wr =", wr2 == wr)
println("type(wr2) =", type(wr2))
