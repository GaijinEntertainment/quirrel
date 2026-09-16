from "debug" import getbuildinfo

let t = getbuildinfo()
let keys = []
foreach (k, v in t) keys.append(k)
keys.sort()
println("keys of getbuildinfo() =", ", ".join(keys))

// the values themselves are build properties, so only their types are stable
println("types of version/charsize/intsize/floatsize/docstring_registry_slots/gc =",
        type(t.version), type(t.charsize), type(t.intsize),
        type(t.floatsize), type(t.docstring_registry_slots), type(t.gc))
