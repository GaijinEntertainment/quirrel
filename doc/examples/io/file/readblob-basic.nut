from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_readblob_basic.tmp"
let f = file(path, "w+")
f.writestring("ammo")

f.seek(0)
println("f.readblob(4).as_string() =", f.readblob(4).as_string())

f.close()
remove(path)
