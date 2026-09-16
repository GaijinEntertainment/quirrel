from "iostream" import blob

let cargo = blob(4)
cargo.readn('i')            // consumes all 4 bytes
println("cargo.eos() =", cargo.eos())
