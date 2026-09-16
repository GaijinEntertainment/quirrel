// true.tochar() is chr(1), a control character - print its code, not the raw byte.
let c = true.tochar()
println("c.len() =", c.len())
println("c[0] =", c[0])
println("false.tochar()[0] =", false.tochar()[0])
