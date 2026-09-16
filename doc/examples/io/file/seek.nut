from "io" import file
from "system" import getenv, remove

let path = $"{getenv("TEMP") ?? getenv("TMP") ?? "/tmp"}/qrl_io_file_seek.tmp"
let f = file(path, "w+")
f.writestring("abcdef")

println("f.seek(-2, 'e') =", f.seek(-2, 'e'))   // two bytes back from the real end
println("f.readn('b') =", f.readn('b'))      // 'e' = 101

println("f.seek(100, 'b') =", f.seek(100, 'b'))  // past the real end is not an error for a file
println("f.tell() =", f.tell())

try { f.seek(0, 'z'); }
catch (e) { println("f.seek(0, 'z') throws:", e); } // only the origin character is validated

f.close()
remove(path)
