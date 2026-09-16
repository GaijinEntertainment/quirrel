let modules = require("modules")
let names = modules.get_native_module_names()

// The set of native modules depends on the host binary, so this checks an
// invariant instead of printing the (host-specific) list: "modules" and
// "types" are always registered together by the module system itself.
local hasModules = false
local hasTypes = false
foreach (name in names) {
  if (name == "modules") hasModules = true
  else if (name == "types") hasTypes = true
}
println("typeof names =", typeof names)
println("hasModules && hasTypes =", hasModules && hasTypes)
