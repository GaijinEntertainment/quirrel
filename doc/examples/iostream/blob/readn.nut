from "iostream" import blob

let b = blob(0)
// one byte at a time, so the printed values cannot depend on byte order
b.writen(0x41, 'b')
b.writen(0x42, 'b')
b.seek(0)

println("b.readn('b') =", b.readn('b'))   // readn converts and moves the cursor
println("b.tell() after readn('b') =", b.tell())

println("b[1] =", b[1])           // indexing reads a byte but never moves the cursor
println("b.tell() after b[1] =", b.tell())

b.seek(1)               // only 1 byte remains, readn('i') needs 4
try {
  b.readn('i')
} catch (e) {
  println("b.readn('i') throws:", e)
}
