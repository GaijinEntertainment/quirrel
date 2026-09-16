from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_tell_basic.tmp"
let f = file(path, "w+")
f.writestring("ammo")
println("f.tell() =", f.tell())

f.close()
remove(path)
