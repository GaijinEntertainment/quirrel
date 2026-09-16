from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_flush.tmp"
let f = file(path, "w")
f.writestring("data")
println("f.flush() =", f.flush())   // non-null: the OS buffer for this handle was flushed

f.close()
try { f.flush(); }
catch (e) { println("f.flush() after close throws:", e); }   // flushing a closed file is still an invalid stream

remove(path)
