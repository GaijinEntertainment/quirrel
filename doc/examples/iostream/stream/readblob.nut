from "iostream" import blob

let b = blob(4)
b.writen(0x01020304, 'i')
b.seek(2)
let r = b.readblob(10) // size is capped against len (4), not what remains (2)
println("r.len() =", r.len())         // only the 2 bytes actually available come back
println("b.tell() =", b.tell())

try { b.readblob(-1) } catch (e) { println("b.readblob(-1) throws:", e) }

b.seek(0, 'e')
try { b.readblob(1) } catch (e) { println("b.readblob(1) at eos throws:", e) } // cursor already at len
