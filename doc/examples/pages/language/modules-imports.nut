from "math" import PI
import "string" as Str

println("PI =", PI)
println(Str.format("hero: %s", "tank"))

// root table names are not visible as bare identifiers in module code
getroottable().activeMissionId <- "M07"
println("getroottable().activeMissionId =", getroottable().activeMissionId)

println("type(require(\"math\")) =", type(require("math")))
println("require_optional(\"no_such_native_module\") =", require_optional("no_such_native_module"))
