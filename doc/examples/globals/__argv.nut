let root = getroottable()

// module code cannot see root table names on its own
println("__argv in root =", "__argv" in root)
println("type(root.__argv) =", type(root.__argv))

// the interpreter's own options are in there too, so the length is not fixed:
// printing it would only record how this one run was launched
println("root.__argv.len() >= 2 =", root.__argv.len() >= 2)
