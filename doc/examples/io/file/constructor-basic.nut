from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_constructor_basic.tmp"
let save = file(path, "w")
println("typeof save =", typeof save)

save.close()
remove(path)
