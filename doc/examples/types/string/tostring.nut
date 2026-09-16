// a string converted to a string is the same string.
let s = "hello"
println("s.tostring() == s =", s.tostring() == s)

try { s.tostring(1) } catch (e) println("s.tostring(1) throws:", e)
