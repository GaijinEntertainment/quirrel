from "string" import lstrip

println("lstrip(\"  hi  \") =", $"[{lstrip("  hi  ")}]")

// nothing at the front to strip: the string comes back unchanged
println("lstrip(\"hi\") =", $"[{lstrip("hi")}]")
