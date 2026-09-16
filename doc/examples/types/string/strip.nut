println("\"  hi  \".strip() =", $"[{"  hi  ".strip()}]")

// same implementation as the string module function, just called as a method.
let strmod = require("string")
println("\"  hi  \".strip() == strmod.strip(\"  hi  \") =", "  hi  ".strip() == strmod.strip("  hi  "))
