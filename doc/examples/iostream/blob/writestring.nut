from "iostream" import blob

let b = blob(0)          // empty: writing to it must grow it
println("b.writestring(\"hi\") =", b.writestring("hi"))
println("b.len() =", b.len())
println("b.as_string() =", b.as_string())
