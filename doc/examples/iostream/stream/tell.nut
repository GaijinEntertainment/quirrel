from "iostream" import blob

let b = blob(4)
println("b.tell() at start =", b.tell()) // starts at 0
b.readn('i')
println("b.tell() after readn('i') =", b.tell()) // advanced by the 4 bytes just read
b.seek(1)
println("b.tell() after seek(1) =", b.tell())
b.writen(1, 'c')
println("b.tell() after writen('c') =", b.tell()) // advanced by the 1 byte just written
