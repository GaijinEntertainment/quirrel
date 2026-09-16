from "iostream" import blob

let cargo = blob(2)
cargo.writen(0x1234, 's')
cargo.swap2()
println("cargo[0], cargo[1] =", $"{cargo[0]},{cargo[1]}")
