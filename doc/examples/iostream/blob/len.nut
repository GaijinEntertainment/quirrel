from "iostream" import blob

let b = blob(4)
println("b.len() at start =", b.len())
b.seek(0, 'e')
b.writen(1, 'b')   // writing past the end grows len
println("b.len() after writen past end =", b.len())

b.seek(0)
println("b.len() after seek(0) =", b.len())  // seeking never changes len, only tell
