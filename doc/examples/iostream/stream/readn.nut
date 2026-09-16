from "iostream" import blob

let b = blob(0)                 // grows to fit as each write extends past the end
b.writen(0x0102030405060708, 'l') // 8-byte native integer
b.writen(-7, 'c')                 // 1-byte signed
println("b.len() =", b.len())                  // 8 + 1 = 9

b.seek(0)
println("b.readn('l') == 0x0102030405060708 =", b.readn('l') == 0x0102030405060708)
println("b.readn('c') =", b.readn('c'))

b.seek(-1, 'e')                   // 1 byte left before len, 'i' needs 4
try { b.readn('i') } catch (e) { println("b.readn('i') throws:", e) }
try { b.readn('z') } catch (e) { println("b.readn('z') throws:", e) } // 'z' is not a format code
