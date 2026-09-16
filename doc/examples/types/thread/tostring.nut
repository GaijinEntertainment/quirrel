function body() { suspend() }
let t = newthread(body)
let s = t.tostring()
println("s.slice(0, 9) =", s.slice(0, 9)) // "(thread :"
try { t.tostring(1) } catch(e) { println("t.tostring(1) throws:", e) }
