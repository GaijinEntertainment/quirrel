from "string" import startswith

println("startswith(\"hello world\", \"hello\") =", startswith("hello world", "hello"))
println("startswith(\"hello\", \"hello world\") =", startswith("hello", "hello world")) // prefix longer than str: always false

// the empty prefix always matches
println("startswith(\"anything\", \"\") =", startswith("anything", ""))
