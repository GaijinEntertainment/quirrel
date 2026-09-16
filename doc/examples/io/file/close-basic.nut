from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_close_basic.tmp"
let save = file(path, "w")
save.close()

try { save.tell() } catch (e) { println("save.tell() after close throws:", e) }

remove(path)
