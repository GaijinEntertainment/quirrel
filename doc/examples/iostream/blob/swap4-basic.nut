from "iostream" import blob

let cargo = blob(4)
cargo.writen(0x11223344, 'i')
cargo.swap4()
println("cargo[0], cargo[1], cargo[2], cargo[3] =", $"{cargo[0]},{cargo[1]},{cargo[2]},{cargo[3]}")
