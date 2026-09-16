from "io" import file
from "iostream" import blob
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_writeblob_basic.tmp"
let cargo = blob(0)
cargo.writestring("ammo")

let f = file(path, "wb")
println("f.writeblob(cargo) =", f.writeblob(cargo))

f.close()
remove(path)
