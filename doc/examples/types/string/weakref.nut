let s = "hello"
let w = s.weakref()
println("type(w) =", type(w))
println("w.ref() == s =", w.ref() == s)
