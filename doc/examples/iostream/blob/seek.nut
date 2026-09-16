from "iostream" import blob

let b = blob(4)
println("b.seek(2, 'b') =", b.seek(2, 'b'))   // ok: returns 0, cursor now 2
println("b.tell() after seek(2, 'b') =", b.tell())
println("b.seek(-1, 'c') =", b.seek(-1, 'c'))  // relative to the current position
println("b.tell() after seek(-1, 'c') =", b.tell())
println("b.seek(0, 'e') =", b.seek(0, 'e'))   // relative to len
println("b.tell() after seek(0, 'e') =", b.tell())

println("b.seek(100, 'b') =", b.seek(100, 'b')) // out of range: returns -1, does not throw
println("b.tell() after seek(100, 'b') =", b.tell())         // cursor is unchanged

try {
  b.seek(0, 'z')
} catch (e) {
  println("b.seek(0, 'z') throws:", e)
}
