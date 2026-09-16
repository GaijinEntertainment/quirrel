from "string" import strip

println("strip(\"  hello  \") =", $"[{strip("  hello  ")}]")
println("strip(\"\\t\\nhello\\r\\n\") =", $"[{strip("\t\nhello\r\n")}]")

// all white space strips to an empty string
println("strip(\"   \") =", $"[{strip("   ")}]")

// bytes outside the white-space set are left alone
println("strip(\"--hi--\") =", $"[{strip("--hi--")}]")

// same operation, called as a method
println("\"  hi  \".strip() =", "  hi  ".strip())
