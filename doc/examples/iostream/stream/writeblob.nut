from "iostream" import blob

let src = blob(3)
src.writen(1, 'c')
src.writen(2, 'c')
src.writen(3, 'c')

let dst = blob(0)
println("dst.writeblob(src) =", dst.writeblob(src)) // returns the number of bytes written
println("dst.len() =", dst.len())

class Foo {}
try { dst.writeblob(Foo()) } catch (e) { println("dst.writeblob(Foo()) throws:", e) } // an instance, but not a blob
