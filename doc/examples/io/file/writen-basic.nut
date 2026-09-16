from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_writen_basic.tmp"
let f = file(path, "w+b")
f.writen(200, 'i')

f.seek(0)
println("f.readn('i') =", f.readn('i'))

f.close()
remove(path)
