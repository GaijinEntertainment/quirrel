println("\"hello\".contains(\"ell\") =", "hello".contains("ell"))
println("\"hello\".contains(\"xyz\") =", "hello".contains("xyz"))

// unlike indexof, a negative start does not wrap: it just cannot match.
println("\"hello\".contains(\"l\", -2) =", "hello".contains("l", -2))
