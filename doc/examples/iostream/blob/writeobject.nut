from "iostream" import blob

let b = blob(0)               // empty: serializing grows it
b.writeobject([1, 2, 3])
println("b.len() > 0 =", b.len() > 0)

b.seek(0)
let back = b.readobject()
println("back.len(), back[0], back[1], back[2] =", $"{back.len()}:{back[0]},{back[1]},{back[2]}")
