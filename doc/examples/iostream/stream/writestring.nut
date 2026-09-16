from "iostream" import blob

let b = blob(0)
println("b.writestring(\"hi\") =", b.writestring("hi")) // the number of bytes written
println("b.len() after writestring(\"hi\") =", b.len())

let e = "\xC3\xA9"       // one letter, 2 UTF-8 bytes
println("b.writestring(e) =", b.writestring(e)) // counts bytes, not code points
println("b.len() after writestring(e) =", b.len())
