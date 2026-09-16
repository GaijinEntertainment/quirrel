from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_readblob.tmp"
let f = file(path, "w+")
f.writestring("abcde")

f.seek(3)
println("f.readblob(5).len() =", f.readblob(5).len())  // only 2 bytes remain, so fewer come back than asked

try { f.readblob(1); }
catch (e) { println("f.readblob(1) throws:", e); }     // now truly at the end: nothing at all to read

f.close()
remove(path)
