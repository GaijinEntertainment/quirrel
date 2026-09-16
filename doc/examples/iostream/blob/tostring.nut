from "iostream" import blob

let s = blob(2).tostring()
println("type(s) =", type(s))
println("s.len() > 10 =", s.len() > 10)     // the address makes the string long
println("s.slice(0, 6) =", s.slice(0, 6))    // only the fixed prefix is safe to print
