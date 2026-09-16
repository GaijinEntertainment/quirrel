let modules = require("modules")

// Runs when this script's module manager is torn down: normally at process
// exit, or before a script is hot-reloaded during development.
modules.on_module_unload(function(is_app_closing) {
  println($"unload, is_app_closing={is_app_closing}")
})

println("script body done")
// Nothing else to do: the callback above fires after this, when the host
// closes the VM, so its output always comes last.
