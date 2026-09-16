from "datetime" import clock

// the reading itself varies, so only its type and sign are stable
let t = clock()
println("type(t) =", type(t))
println("t >= 0.0 =", t >= 0.0)
