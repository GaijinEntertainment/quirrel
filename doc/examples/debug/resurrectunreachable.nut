from "debug" import resurrectunreachable

println("resurrectunreachable() == null =", resurrectunreachable() == null) // nothing pending yet

// a reference cycle: refcounting cannot free this by itself
local a = {}
local b = {}
a.other <- b
b.other <- a
a = null
b = null

let found = resurrectunreachable()
println("type(found) =", type(found))
println("found.len() > 0 =", found.len() > 0)
