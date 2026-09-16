from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_readn.tmp"
let f = file(path, "w+b")
f.writen(0x01020304, 'i')
f.seek(0)
println("f.readn('i') =", f.readn('i'))   // native byte order, same value back

f.seek(3)                // only 1 byte left before len, 'i' needs 4
try { f.readn('i'); }
catch (e) { println("f.readn('i') at offset 3 throws:", e); }

f.close()
remove(path)
