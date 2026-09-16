let modules = require("modules")
let names = modules.get_native_module_names()

println("names.contains(\"modules\") =", names.contains("modules"))
