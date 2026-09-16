from "iostream" import blob

let b = blob(4)
println("b.seek(2) =", b.seek(2))      // default origin 'b': absolute position
println("b.tell() after seek(2) =", b.tell())
println("b.seek(1, 'c') =", b.seek(1, 'c')) // relative to the current position
println("b.tell() after seek(1, 'c') =", b.tell())
println("b.seek(0, 'e') =", b.seek(0, 'e')) // relative to the end
println("b.tell() after seek(0, 'e') =", b.tell())

println("b.seek(5) =", b.seek(5))      // past len: out of range
println("b.tell() after seek(5) =", b.tell())       // cursor did not move

try { b.seek(0, 'z') } catch (e) { println("b.seek(0, 'z') throws:", e) }
