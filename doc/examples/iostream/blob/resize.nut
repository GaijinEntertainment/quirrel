from "iostream" import blob

let b = blob(4)
b.writen(0x11223344, 'i')
b.seek(0)
println("bytes before resize =", $"{b[0]},{b[1]},{b[2]},{b[3]}")

// shrinking then growing back does not bring the old tail back
b.resize(1)
b.resize(4)
println("bytes after resize(1) then resize(4) =", $"{b[0]},{b[1]},{b[2]},{b[3]}")

try {
  b.resize(-1)
} catch (e) {
  println("b.resize(-1) throws:", e)
}
