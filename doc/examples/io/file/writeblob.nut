from "io" import file
from "iostream" import blob
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_writeblob.tmp"
let b = blob(3)
b.writen(0x41, 'b')
b.writen(0x42, 'b')
b.writen(0x43, 'b')

let f = file(path, "wb")
println("f.writeblob(b) =", f.writeblob(b))  // all of it goes, and the count written comes back
f.close()

let r = file(path, "rb")
println("r.len() =", r.len())
r.close()
remove(path)
