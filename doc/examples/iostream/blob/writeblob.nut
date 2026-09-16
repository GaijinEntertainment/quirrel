from "iostream" import blob

let src = blob(3)
src[0] = 7; src[1] = 8; src[2] = 9

let dst = blob(0)        // empty: writing to it must grow it
println("dst.writeblob(src) =", dst.writeblob(src))
println("dst.len() =", dst.len())
println("dst[0], dst[1], dst[2] =", $"{dst[0]},{dst[1]},{dst[2]}")
