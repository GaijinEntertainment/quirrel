println("\"abc\".hasindex(0) =", "abc".hasindex(0))
println("\"abc\".hasindex(2) =", "abc".hasindex(2))
println("\"abc\".hasindex(3) =", "abc".hasindex(3))    // one past the end

// negative index is simply out of range here, unlike slice.
println("\"abc\".hasindex(-1) =", "abc".hasindex(-1))
