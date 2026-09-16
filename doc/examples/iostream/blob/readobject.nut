from "iostream" import blob

local b = blob(0)
b.writeobject({ a = 1, b = "x" })
b.seek(0)                       // rewind before reading back
let back = b.readobject()
println($"{back.a},{back.b}")
