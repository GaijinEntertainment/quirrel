from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_readobject.tmp"
let w = file(path, "wb")
w.writeobject([1, 2, 3])
w.close()

let r = file(path, "rb")
let v = r.readobject()      // the array survives the round trip through disk
println(v[0] + v[1] + v[2])
r.close()

remove(path)
