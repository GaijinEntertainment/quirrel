function inner() { suspend() }
function body() { inner() }
let t = newthread(body)
t.call()
println("t.getstackinfos(0).func =", t.getstackinfos(0).func)
println("t.getstackinfos(1).func =", t.getstackinfos(1).func)
println("t.getstackinfos(99) =", t.getstackinfos(99)) // out of range level
