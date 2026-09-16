from "iostream" import blob

let squadIds = blob(0)
squadIds.writeobject([101, 102, 103])

squadIds.seek(0)
let back = squadIds.readobject()
println("back[0], back[1], back[2] =", $"{back[0]},{back[1]},{back[2]}")
