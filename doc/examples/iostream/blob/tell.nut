from "iostream" import blob

let b = blob(4)
println("b.tell() at start =", b.tell())
b.readn('i')
println("b.tell() after readn('i') =", b.tell())   // readn advanced the cursor by 4 bytes

b[0]                 // indexing reads a byte but never moves the cursor
println("b.tell() after b[0] =", b.tell())
