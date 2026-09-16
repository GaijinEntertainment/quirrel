from "string" import endswith

println("endswith(\"hello world\", \"world\") =", endswith("hello world", "world"))
println("endswith(\"world\", \"hello world\") =", endswith("world", "hello world")) // suffix longer than str: always false

// the empty suffix always matches
println("endswith(\"anything\", \"\") =", endswith("anything", ""))
