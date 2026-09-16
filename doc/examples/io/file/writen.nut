from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_writen.tmp"
let f = file(path, "w+b")
f.writen(0x01020304, 'i')
println("f.len() after writen =", f.len())      // 4 bytes for a 32-bit 'i', same sizes as iostream.stream.writen

f.close()
remove(path)
