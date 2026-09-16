from "iostream" import blob

let b = blob(0)         // empty; every write below extends it
b.writen(0x11223344, 'i') // 4 bytes
println("b.len() after writen('i') =", b.len())
b.writen(2.5, 'f')        // writing past the end grows the blob, never an error
println("b.len() after writen('f') =", b.len())

b.seek(0)
println("b.readn('i') == 0x11223344 =", b.readn('i') == 0x11223344)
println("b.readn('f') =", b.readn('f'))

try { b.writen(1, 'z') } catch (e) { println("b.writen(1, 'z') throws:", e) } // 'z' is not a format code
