from "iostream" import blob

let cargo = blob(0)
cargo.writestring("gold")
cargo.seek(0)
println("cargo.readblob(4).as_string() =", cargo.readblob(4).as_string())
