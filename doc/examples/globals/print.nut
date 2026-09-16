print("no")
print("gap")            // two calls, no newline between: text runs together
println()

print("a", "b", "c")    // one call, several arguments: joined with a single space
println()

println("type(print(\"x\")):")
println(type(print("x")))  // print's own return value
