let t = {a=1, b=null}
println("t.rawin(\"a\") =", t.rawin("a"))
println("t.rawin(\"missing\") =", t.rawin("missing"))
println("t.rawin(\"b\") =", t.rawin("b"))       // present with a null value: still true
println("\"a\" in t:", "a" in t)           // the 'in' operator agrees for a plain table
