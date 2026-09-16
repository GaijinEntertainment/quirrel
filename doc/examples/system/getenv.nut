from "system" import getenv

// a name this unlikely to be set keeps the "not found" path the same everywhere
let v = getenv("QUIRREL_REFDOC_EXAMPLE_UNSET_VAR")
println("v =", v)
println("type(v) =", type(v))
