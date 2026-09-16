from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_eos.tmp"
let f = file(path, "w+")
f.writestring("ab")
println("f.eos() after write =", f.eos())    // just wrote up to the end, so the cursor is there too

f.seek(0)
println("f.eos() after seek(0) =", f.eos())    // rewound, one byte away from the end

f.close()
remove(path)
