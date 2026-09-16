from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_tell.tmp"
let w = file(path, "w")
w.writestring("abcde")
w.close()

let a = file(path, "a")
a.writestring("Z")
println("a.tell() after an append =", a.tell())  // the write landed at the real end
a.seek(2)
println("a.tell() after seek(2) =", a.tell())

a.close()
remove(path)
