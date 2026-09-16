from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_close.tmp"
let f = file(path, "w")
f.close()
f.close()                  // closing twice is harmless

try { f.tell(); }
catch (e) { println(e); }  // any other method now sees an invalid stream

remove(path)
