let t = {a=1, b=null}
println("t.hasindex(\"a\") =", t.hasindex("a"))
println("t.hasindex(\"missing\") =", t.hasindex("missing"))
println("t.hasindex(\"b\") =", t.hasindex("b"))       // present with a null value: still true
println("t.hasindex(\"a\") == t.rawin(\"a\"):", t.hasindex("a") == t.rawin("a"))  // identical on a table
