from "iostream" import blob

let b = blob(3)
println("b.len() at start =", b.len())
b.seek(0, 'e')
b.writen(1, 'l') // writes 8 bytes past the current len of 3, growing it
println("b.len() after writen =", b.len())
b.seek(0)
println("b.len() after seek(0) =", b.len())  // seeking never changes len
println("b.tell() after seek(0) =", b.tell()) // len and tell are independent
