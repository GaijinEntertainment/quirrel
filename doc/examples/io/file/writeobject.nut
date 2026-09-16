from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_writeobject.tmp"
let w = file(path, "wb")
w.writeobject("hello")
println("w.tell() > 0 =", w.tell() > 0)   // the marker and payload took some real bytes
w.close()

let r = file(path, "rb")
println("r.readobject() =", r.readobject())  // read it back with io.file.readobject
r.close()

remove(path)
