println("\"hello\".slice(1, 3) =", "hello".slice(1, 3))
println("\"hello\".slice(-3) =", "hello".slice(-3))       // negative index counts from the end

// out-of-range and inverted ranges clamp instead of throwing.
println("\"hello\".slice(10) =", $"[{"hello".slice(10)}]")
println("\"hello\".slice(3, 1) =", $"[{"hello".slice(3, 1)}]")
