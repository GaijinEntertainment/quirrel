from "iostream" import blob

let empty = blob()          // size defaults to 0
println("empty.len() =", empty.len())

let b = blob(3)             // new bytes are zero-filled
println("b[0], b[1], b[2] =", $"{b[0]},{b[1]},{b[2]}")

try {
  blob(-1)
} catch (e) {
  println("blob(-1) throws:", e)
}
