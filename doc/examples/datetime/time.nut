from "datetime" import time

// the wall clock never runs backwards between two calls in the same script
let t1 = time()
let t2 = time()
println("type(t1) =", type(t1))
println("t2 >= t1 =", t2 >= t1)
