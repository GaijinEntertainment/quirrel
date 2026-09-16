from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_seek_basic.tmp"
let f = file(path, "w+")
f.writestring("ammo42")

f.seek(4)                // absolute offset from the start
println("f.readblob(2).as_string() =", f.readblob(2).as_string())

f.close()
remove(path)
