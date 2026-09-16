println("\"banana\".replace(\"an\", \"AN\") =", "banana".replace("an", "AN"))   // every occurrence, not just the first

// the inserted text is never rescanned for further matches.
println("\"aaa\".replace(\"a\", \"aa\") =", "aaa".replace("a", "aa"))

println("\"banana\".replace(\"\", \"X\") =", "banana".replace("", "X"))      // empty `from`: unchanged
