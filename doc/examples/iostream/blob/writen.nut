from "iostream" import blob

let b = blob(0)         // empty: writing past the end must grow it
b.writen(0x11223344, 'i')
println("b.len() =", b.len())
