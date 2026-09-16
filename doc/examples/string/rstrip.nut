from "string" import rstrip

println("rstrip(\"  hi  \") =", $"[{rstrip("  hi  ")}]")

// nothing at the back to strip: the string comes back unchanged
println("rstrip(\"hi\") =", $"[{rstrip("hi")}]")
