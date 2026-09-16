function courierRun() { return suspend() }
let t = newthread(courierRun)
t.call()
println("t.wakeup(\"advance\") =", t.wakeup("advance"))
