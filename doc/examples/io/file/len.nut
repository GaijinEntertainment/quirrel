from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_len.tmp"
let f = file(path, "w+")
f.writestring("abcde")
println("f.len() after write =", f.len())     // total size on disk

f.seek(2)
println("f.len() after seek(2) =", f.len())     // len does not depend on the cursor, unlike tell

f.close()
remove(path)
