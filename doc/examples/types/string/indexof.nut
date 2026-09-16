println("\"hello\".indexof(\"l\") =", "hello".indexof("l"))
println("\"hello\".indexof(\"l\", 3) =", "hello".indexof("l", 3))

// not found is null, not -1.
let r = "hello".indexof("z")
println("\"hello\".indexof(\"z\") == null =", r == null)

try { "hello".indexof("") } catch (e) println("\"hello\".indexof(\"\") throws:", e)
