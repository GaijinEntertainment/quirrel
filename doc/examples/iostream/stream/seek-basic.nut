from "iostream" import blob

let cargo = blob(4)
cargo.seek(2)
println("cargo.tell() =", cargo.tell())
