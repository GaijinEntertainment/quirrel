from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_writestring.tmp"
let f = file(path, "wb")             // binary mode: no line-ending translation
println("f.writestring(\"ab\\ncd\") =", f.writestring("ab\ncd"))       // returns the character count, 5
f.close()

let r = file(path, "rb")
println("r.len() =", r.len())                       // same 5 bytes landed on disk
r.close()
remove(path)
