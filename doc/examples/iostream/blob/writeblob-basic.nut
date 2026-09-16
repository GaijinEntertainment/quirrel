from "iostream" import blob

let src = blob(0)
src.writestring("ammo")

let dst = blob(0)
println("dst.writeblob(src) =", dst.writeblob(src))
println("dst.as_string() =", dst.as_string())
