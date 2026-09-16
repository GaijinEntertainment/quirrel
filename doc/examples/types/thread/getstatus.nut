function body() { suspend(); return "done" }
let t = newthread(body)

println("before the first call =", t.getstatus())
t.call()
println("after call(), parked on suspend =", t.getstatus())
t.wakeup()
// back to idle: a thread has no permanent dead state, unlike a generator
println("after wakeup(), body finished =", t.getstatus())
