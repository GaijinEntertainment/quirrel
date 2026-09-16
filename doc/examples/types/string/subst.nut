println("\"hi {0}, you are {1}\".subst(\"bob\", 42) =", "hi {0}, you are {1}".subst("bob", 42))
println("\"{n} plus one\".subst({n = 41}) =", "{n} plus one".subst({n = 41}))

// an unmatched placeholder is left as-is.
println("\"{5}\".subst(\"x\") =", "{5}".subst("x"))

try { "hi".subst() } catch (e) println("\"hi\".subst() throws:", e)
