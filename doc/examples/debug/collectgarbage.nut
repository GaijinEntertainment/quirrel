from "debug" import collectgarbage

// two tables that reference each other: refcounting alone can never free this,
// since neither slot ever drops to zero references on its own
local a = {}
local b = {}
a.other <- b
b.other <- a
a = null
b = null

let reclaimed = collectgarbage()
println(reclaimed > 0)
